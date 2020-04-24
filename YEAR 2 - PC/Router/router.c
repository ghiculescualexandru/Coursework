#include "skel.h"
#include "queue.h"
#include <inttypes.h>
#include <stdlib.h>

static const char RTABLE_FILE[] = "rtable.txt";

table_entry *rtable;
int rtable_size;

arp_entry *arp_table;
int arp_table_size;

queue q;

/*
*	Description: this function checks if a destionation is valid, if the 
*	formula is right(destination ip ANDed with the mask equals the prefix).
*
*	Parameters: the destination ip, its index in the routing table.
*	Return: 1 if valid, 0 otherwise.
*/
int valid_dest(uint32_t dest_ip, int idx) {
	if ((dest_ip & ntohl(rtable[idx].mask)) == ntohl(rtable[idx].prefix)) {
		return 1;
	}

	return 0;
}

/*
*	Description: this function implements a binary search on the routing 
*	table, also looking for the longest possible mask. 
*
*	Parameters: the destination ip, low and high for binary search.
*	Return: a pointer to the best route entry, if any, NULL otherwise.
*/
table_entry *binary_search(uint32_t dest_ip, int low, int high) {
	if (low < high) {
		int mid = low + (high - low) / 2;
		/* Checking if mid is the right destination ip. */
		if (valid_dest(dest_ip, mid)) {
			/* If it is the right ip, it looks for a greater mask,
			   which still matches the formula.*/
			while (valid_dest(dest_ip, mid + 1)) {
				if (++mid == rtable_size) {
					break;
				}
			}
			return &rtable[mid];
		}
		/* If the ANDed value is less, searches left side, right otherwise. */
		if ((dest_ip & ntohl(rtable[mid].mask)) < ntohl(rtable[mid].prefix)) {
			return binary_search(dest_ip, low, mid - 1);
		} else {
			return binary_search(dest_ip, mid + 1, high);
		}
	}

	return NULL;
}

/*
*	Description: This function looks up the best route, if any, using 
*	binary search implementation.
*
*	Parameters: the destination ip.
*	Return: a pointer to the best route entry if any, NULL otherwise.
*/
table_entry *lookup_rtable(__u32 ip) {
	return binary_search(ip, 0, rtable_size - 1);
}

/*
*	Description: This function looks up the arp entry matching the destination.
*
*	Parameters: the destination ip.
*	Return: a pointer to the arp entry, if any, NULL otherwise.
*/
arp_entry *get_arp_entry(__u32 ip) {
	for (int i = 0; i < arp_table_size; i++) {
		if (ip == arp_table[i].ip) {
			return &arp_table[i];
		}
	}

    return NULL;
}

/*
*	Description: This function computes the checksum for a header.
*
*	Parameters: the header and its size.
*	Return: the check sum.
*/
uint16_t ip_checksum(void *vdata, size_t length) {
	char *data = (char*)vdata;
	uint64_t acc = 0xffff;

	unsigned int offset = ((uintptr_t)data) & 3;

	if (offset) {
		size_t count = 4 - offset;

		if (count>length) { 
			count = length; 
		}

		uint32_t word = 0;
		memcpy(offset + (char*)&word, data, count);
		acc += ntohl(word);
		data += count;
		length -= count;
	}

	char* data_end = data + (length &~ 3);

	while (data != data_end) {
		uint32_t word;
		memcpy(&word, data, 4);
		acc += ntohl(word);
		data += 4;
	}

	length &= 3;

	if (length) {
		uint32_t word = 0;
		memcpy(&word, data, length);
		acc += ntohl(word);
	}

	acc = (acc & 0xffffffff) + (acc >> 32);
	while (acc >> 16) {
		acc = (acc & 0xffff) + (acc >> 16);
	}

	if (offset & 1) {
		acc = ((acc & 0xff00) >> 8)|((acc & 0x00ff) << 8);
	}

	return htons(~acc);
}

/*
*	Description: merges two arrays of table entriy elements in ascending order.
*
*	Parameters: left, mid and right for merge sort.
*/
void merge_halves(int l, int m, int r) {

    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 

    table_entry L[n1], R[n2]; 
  
    for (i = 0; i < n1; i++) {
        L[i] =rtable[l + i]; 
    }

    for (j = 0; j < n2; j++) {
        R[j] =rtable[m + 1+ j]; 
    }
  
    i = 0; 
    j = 0;
    k = l; 
    /* Sorting in ascending order by prefix, if equal, sorting
    *  in ascending order by mask. */
    while (i < n1 && j < n2) {
        if (ntohl(L[i].prefix) < ntohl(R[j].prefix)) { 
           rtable[k] = L[i]; 
            i++; 
        } else if (ntohl(L[i].prefix) > ntohl(R[j].prefix)) { 
           rtable[k] = R[j]; 
            j++; 
        } else if (ntohl(L[i].prefix) == ntohl(R[j].prefix)) {
        	if (ntohl(L[i].mask) < ntohl(R[j].mask)) {
        	rtable[k] = L[i]; 
            	i++; 
        	} else {
        	rtable[k] = R[j]; 
            	j++; 
        	}
        }
        k++; 
    } 
  
    while (i < n1) { 
       rtable[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    while (j < n2) { 
        rtable[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
 
/*
*	Description: this function implements the merge sort for 
*	an array with table entries.
*
*	Parameters: low and high for merge sort.
*/ 
void merge_sort(int low, int high) {
    if (low < high) {
        int mid = low +(high - low) / 2; 
  
        merge_sort(low, mid); 
        merge_sort(mid + 1, high); 
  
        merge_halves(low, mid, high); 
    } 
} 

/*
*	Description: this function sorts the routing table,
*	using a merge-sort.
*/
void sort_rtable() {
	merge_sort(0, rtable_size - 1);
}

/*
*	Description: this function sets the fields for a packet with
*	a given interface, in case of an ip reply.
*
*	Parameters: the source packet.
*	Return: the reply packet.
*/
packet set_icmp_reply(packet *m) {
	packet reply;
	reply.len = sizeof(struct ether_header)
				+ sizeof(struct iphdr)
				+ sizeof(struct icmphdr);
	reply.interface = m->interface;

	return reply;
}

/*
*	Description: this function sets the fields for a packet with
*	a given interface, in case of an arp reply.
*
*	Parameters: the source packet.
*	Return: the reply packet.
*/
packet set_arp_reply(packet *m) {
	packet reply;
	reply.len = sizeof(struct ether_header) + sizeof(struct ether_arp);
	reply.interface = m->interface;

	return reply;
}

/*
*	Description: this function sets the fields for a packet with
*	a given interface, in case of an arp reply.
*
*	Parameters: the destination addres.
*	Return: the request packet.
*/
packet set_arp_request(uint32_t daddr) {
	packet request;

	request.len = sizeof(struct ether_header) + sizeof(struct ether_arp);
	request.interface = lookup_rtable(ntohl(daddr))->interface;

	return request;
}

/*
*	Description: this function sets the fields for an ether header,
*	when setting an arp reply.
*
*	Parameters: the reply packet and the source packet.
*/
void set_ethdr_arp(packet *reply, packet *m) {
	struct ether_header *eth_hdr = (struct ether_header*)m->payload;
	struct ether_header *eth_hdr_aux = (struct ether_header*)reply->payload;

	memcpy(eth_hdr_aux->ether_dhost, eth_hdr->ether_shost, ETH_ALEN);
	get_interface_mac(m->interface , eth_hdr_aux->ether_shost);
	eth_hdr_aux->ether_type = htons(ETH_P_ARP);
}

/*
*	Description: this function sets the fields for an ether header,
*	when setting an arp request.
*
*	Parameters: the request packet.
*/
void set_ethdr_arp_request(packet *request) {
	struct ether_header *eth_hdr_aux = (struct ether_header*)request->payload;

	eth_hdr_aux->ether_type = htons(ETH_P_ARP);
	get_interface_mac(request->interface , eth_hdr_aux->ether_shost);

	for (int i = 0; i < ETH_ALEN; i++) {
		eth_hdr_aux->ether_dhost[i] = 0xFF;

	}
}

/*
*	Description: this function sets the fields for an arp header.
*
*	Parameters, the reply packet, the source packet and the arp operation.
*/
void set_arphdr(packet *reply, packet *m, int ar_op) {
	struct ether_arp *arp_hdr = (struct ether_arp*)(m->payload
								+ sizeof(struct ether_header));
	struct ether_arp *arp_hdr_aux = (struct ether_arp*)(reply->payload
								+ sizeof(struct ether_header));

	memcpy(arp_hdr_aux->arp_tha, arp_hdr->arp_sha, ETH_ALEN);
	memcpy(arp_hdr_aux->arp_spa, arp_hdr->arp_tpa, 4);
	memcpy(arp_hdr_aux->arp_tpa, arp_hdr->arp_spa, 4);

	arp_hdr_aux->ea_hdr.ar_hrd = arp_hdr->ea_hdr.ar_hrd;
	arp_hdr_aux->ea_hdr.ar_pro = arp_hdr->ea_hdr.ar_pro;
	arp_hdr_aux->ea_hdr.ar_hln = 6;
	arp_hdr_aux->ea_hdr.ar_pln = 4;
	arp_hdr_aux->ea_hdr.ar_op = htons(ar_op);
	get_interface_mac(m->interface, arp_hdr_aux->arp_sha);
}

/*
*	Description: this function sets the fields for an arp header.
*
*	Parameters, the request packet, the destination address, the arp operation.
*/
void set_arphdr_request(packet *request, uint32_t daddr, int ar_op) {
	struct ether_arp *arp_hdr_aux = (struct ether_arp*)(request->payload
									+ sizeof(struct ether_header));

	arp_hdr_aux->ea_hdr.ar_hrd = htons(1);
	arp_hdr_aux->ea_hdr.ar_pro = htons(ETH_P_IP);
	arp_hdr_aux->ea_hdr.ar_hln = 6;
	arp_hdr_aux->ea_hdr.ar_pln = 4;

	arp_hdr_aux->ea_hdr.ar_op = htons(ar_op);
	get_interface_mac(request->interface, arp_hdr_aux->arp_sha);

	uint32_t spa = inet_addr(get_interface_ip(request->interface));
	memcpy(arp_hdr_aux->arp_spa, &spa, 4);
	memcpy(arp_hdr_aux->arp_tpa, &daddr, 4);
}

/*
*	Description: this function sets the fields for an ether header.
*
*	Parameters: the reply packet and the source packet.
*/
void set_ether_header(packet *reply, packet *m) {
	struct ether_header *eth_hdr = (struct ether_header*)m->payload;
	struct ether_header *eth_hdr_aux = (struct ether_header*)reply->payload;

	eth_hdr_aux->ether_type = eth_hdr->ether_type;
	memcpy(eth_hdr_aux->ether_dhost, eth_hdr->ether_shost, ETH_ALEN);
	memcpy(eth_hdr_aux->ether_shost, eth_hdr->ether_dhost, ETH_ALEN);
}

/*
*	Description: this function sets the fields for an ip header.
*
*	Parameters: the reply packet and the source packet.
*/
void set_iphdr(packet *reply, packet *m) {
	struct iphdr *ip_hdr_aux = (struct iphdr*)(reply->payload 
								+ sizeof(struct ether_header));
	struct iphdr *ip_hdr = (struct iphdr*)(m->payload
								+ sizeof(struct ether_header));

	ip_hdr_aux->version = 4;
	ip_hdr_aux->ihl = 5;
	ip_hdr_aux->tos = 0;
	ip_hdr_aux->tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmphdr));
	ip_hdr_aux->id = htons(getpid());
	ip_hdr_aux->frag_off = 0;
	ip_hdr_aux->ttl = 64;
	ip_hdr_aux->protocol = IPPROTO_ICMP;
	ip_hdr_aux->saddr = ip_hdr->daddr;
	ip_hdr_aux->daddr = ip_hdr->saddr;
	ip_hdr_aux->check = 0;
	ip_hdr_aux->check = ip_checksum(ip_hdr_aux, sizeof(struct iphdr));
}

/*
*	Description: this function sets the fields for an icmp header,
*	with a given type.
*
*	Parameters: the reply packet and its type.
*/
void set_icmphdr(packet *reply, uint8_t type) {
	struct icmphdr *icmp_hdr_aux = (struct icmphdr*)(reply->payload
									+ sizeof(struct ether_header)
									+ sizeof(struct iphdr));

	icmp_hdr_aux->code = 0;
	icmp_hdr_aux->type = type;
	icmp_hdr_aux->un.echo.id = htons(getpid());
	icmp_hdr_aux->un.echo.sequence = htons(64);
	icmp_hdr_aux->checksum = 0;
	icmp_hdr_aux->checksum = ip_checksum(icmp_hdr_aux, sizeof(struct icmphdr));
}

/*
*	Description: this function sets an arp reply packet and sends it
*	to a given interface.
*
*	Parameters: the source packet and the type of arp operation.
*/
void send_arp_reply(packet *m, int ar_op) {
	packet reply = set_arp_reply(m);
	set_ethdr_arp(&reply, m);
	set_arphdr(&reply, m, ar_op);
	send_packet(m->interface, &reply);
}

/*
*	Description: this function sets an arp request packet.
*
*	Parameters: the destination address and the type of arp operation.
*/
void send_arp_request(uint32_t daddr, int ar_op) {
	packet request = set_arp_request(daddr);
	set_ethdr_arp_request(&request);
	set_arphdr_request(&request, daddr, ar_op);

	send_packet(request.interface, &request);
}

/*
*	Description: This function sets a reply packet and sends it
*	to a given interface.
*
*	Parameters: the source packet and the type of reply operation.
*/
void send_icmp_reply(packet *m, uint8_t type) {
	packet reply = set_icmp_reply(m);
	set_ether_header(&reply, m);
	set_iphdr(&reply, m);
	set_icmphdr(&reply, type);

	send_packet(m->interface, &reply);		
}

/*
*	Description: this function converts the numbers from an IP
*	to an unsigned long number. 
*
*	Parameters: four bytes to be converted.
*	Return: the converted uint32_t number.
*/
uint32_t ip_to_dec(uint8_t o1, uint8_t o2, uint8_t o3, uint8_t o4) {
	return (o4 << 24) + (o3 << 16) + (o2 << 8) + o1;
}

/*
*	Description: this function reads from an input file all fields
*	for each element of the rtable. It also computes the table size.
*/
void read_rtable() {
	rtable = (table_entry*)malloc(sizeof(table_entry) * 66000);
	if (rtable == NULL) {
		fprintf(stderr, "Malloc failed for Route Table.\n");
		exit(1);
	}
	rtable_size = 0;

	FILE *input_file = fopen(RTABLE_FILE, "r");
	if(input_file == NULL) {
		fprintf(stderr, "Input file could not open.\n");
		exit(1);
	}

	while(!feof(input_file)) {
		uint8_t oct1, oct2, oct3, oct4;

		fscanf(input_file, "%hhu.%hhu.%hhu.%hhu", &oct1, &oct2, &oct3, &oct4);
		rtable[rtable_size].prefix  = ip_to_dec(oct1, oct2, oct3, oct4);

		fscanf(input_file, "%hhu.%hhu.%hhu.%hhu", &oct1, &oct2, &oct3, &oct4);
		rtable[rtable_size].next_hop = ip_to_dec(oct1, oct2, oct3, oct4);

		fscanf(input_file, "%hhu.%hhu.%hhu.%hhu", &oct1, &oct2, &oct3, &oct4);
		rtable[rtable_size].mask = ip_to_dec(oct1, oct2, oct3, oct4);

		fscanf(input_file, "%d", &rtable[rtable_size].interface);

		rtable_size++;
	}

	rtable_size--;
}

/*
*	Description: this function allocates memory for the arp table and sets
*	its size to zero.
*/
void init_arp_table() {
	arp_table = (arp_entry*)malloc(sizeof(arp_entry) * 100);
	if (arp_table == NULL) {
		fprintf(stderr, "Malloc failed for Arp Table.\n");
		exit(1);
	}

	arp_table_size = 0;
}

/*
*	Description: this function sets the router, allocates memory, reads the
*	routing table, initialize the arp table and sorts the routing table.
*/
void set_router() {
	q = queue_create();
	read_rtable();
	init_arp_table();
	sort_rtable();
}

/*
*	Description: this function prints the table entries.
*	Its pourpose is just for correct debugging.
*/
void display_rtable() {
	for(int i = 0; i < rtable_size; i++) {
		fprintf(stdout, "%lu %lu %lu %d\n",
			(unsigned long)rtable[i].prefix,
			(unsigned long)rtable[i].next_hop,
			(unsigned long)rtable[i].mask,
			rtable[i].interface);
	}
}

/*
*	Description: this function prints the table entries.
*	Its pourpose is just for correct debugging.
*/
void display_arp_table() {
	for (int i = 0; i < arp_table_size; i++) {
		fprintf(stdout, "%lu ", (unsigned long)arp_table[i].ip);
		for (int j = 0; j < 6; j++) {
			fprintf(stdout, "%d ", arp_table[i].mac[j]);
		}
		fprintf(stdout, "\n");
	}
}

/*
*	Description: this function enque a packet.
*	
*	Parameters: the packet.
*/
void put_in_queue(packet *m) {
	packet *temp = (packet*)malloc(sizeof(packet));
	
	temp->len = m->len;
	memcpy(temp->payload, m->payload, MAX_LEN);
	temp->interface = m->interface;	

	queue_enq(q, temp);
}

/*
*	Description: this function updates the arp table, when receiving 
*	a packet and doesn't yet have an arp entry in the arp table.
*	It also updates the arp table size.
*
*	Parameters: the packet from which the arp header is used.
*/
void update_arp_table(packet *m) {
	struct ether_arp *arp_hdr = (struct ether_arp*)(m->payload
								+ sizeof(struct ether_header));

	memcpy(arp_table[arp_table_size].mac, arp_hdr->arp_sha, ETH_ALEN);

	arp_table[arp_table_size].ip = ip_to_dec(arp_hdr->arp_spa[0],
		arp_hdr->arp_spa[1], arp_hdr->arp_spa[2],arp_hdr->arp_spa[3]);

	arp_table_size++;
}

/*
*	Description: this function searches for a waiting packet in the queue
*	to check if it now has an arp entry to go to. It uses an auxiliar queue
*	to store the packets with no arp entry.
*/
void send_waiting_packet() {
	arp_entry last_entry = arp_table[arp_table_size - 1];
	queue qaux = queue_create();

	while (!queue_empty(q)) {
		packet *temp = queue_deq(q);

		struct iphdr *ip_hdr = (struct iphdr*)(temp->payload
								+ sizeof(struct ether_header));
		struct ether_header *eth_hdr = (struct ether_header*)(temp->payload);

		if(last_entry.ip == ip_hdr->daddr) {
			memcpy(eth_hdr->ether_dhost, last_entry.mac, 6);
			table_entry *best_route = lookup_rtable(ntohl(ip_hdr->daddr));

			send_packet(best_route->interface, temp);
		} else {
			queue_enq(qaux, temp);
		}
	}

	while (!queue_empty(qaux)) {
		queue_enq(q, queue_deq(qaux));
	}
}

/*
*	Description: this function handles the icmp request with the router
*	as destination.
*
*	Parameters: the source packet.
*/
int manage_echo_request(packet *m, struct iphdr *ip_hdr) {
	if(ip_hdr->protocol == IPPROTO_ICMP) {
		struct icmphdr* icmp_hdr = (struct icmphdr*)(m->payload
									+ sizeof(struct ether_header)
									+ sizeof(struct iphdr));

		if (icmp_hdr->type == ICMP_ECHO) {
			send_icmp_reply(m, ICMP_ECHOREPLY);
			return SENT;
		}
		return LOST;
	}

	return LOST;
}

/*
*	Description: this function handles the ip forwarding, managing icmp replies,
*	and sending the packets if possible.
*
*	Parameters: the source packet and the ether header.
*/
int manage_ip_forwarding(packet *m, struct ether_header *eth_hdr) {
	struct iphdr *ip_hdr = (struct iphdr*)(m->payload
							+ sizeof(struct ether_header));

	if (inet_addr(get_interface_ip(m->interface)) == ip_hdr->daddr) {
		return manage_echo_request(m, ip_hdr);
	}

	if (ip_checksum(ip_hdr, sizeof(struct iphdr)) != 0) {
		return LOST;
	}

	if (ip_hdr->ttl <= 1) {
		send_icmp_reply(m, ICMP_TIME_EXCEEDED);
		return LOST;
	}

	table_entry *best_route = lookup_rtable(ntohl(ip_hdr->daddr));
	if (best_route == NULL) {
		send_icmp_reply(m, ICMP_DEST_UNREACH);
		return LOST;
	}

	ip_hdr->ttl--;
	ip_hdr->check = 0;
	ip_hdr->check = ip_checksum(ip_hdr, sizeof(struct iphdr));

	arp_entry *arp_entry = get_arp_entry(ip_hdr->daddr);
	if (arp_entry == NULL) {
		send_arp_request(ip_hdr->daddr, ARPOP_REQUEST);
		put_in_queue(m);
		return HOLD;
	}

	memcpy(eth_hdr->ether_dhost, arp_entry->mac, 6);
	send_packet(best_route->interface, m);

	return SENT;
}

/*
*	Description: this function handles the arp protocol, sending reply if
*	receiving an arp request, and updating the arp table when receiving an
*	arp reply. It also looks for packets to send after updating the arp table.
*
*	Parameters: the source packet.
*/
int manage_arp_protocol(packet *m) {
	struct ether_arp *arp_hdr = (struct ether_arp*)(m->payload
								+ sizeof(struct ether_header));

	if (ntohs(arp_hdr->ea_hdr.ar_op) == ARPOP_REQUEST){
		send_arp_reply(m, ARPOP_REPLY);
	} else if (ntohs(arp_hdr->ea_hdr.ar_op) == ARPOP_REPLY) {
		update_arp_table(m);
		send_waiting_packet();
	}			
	
	return SENT;
}

/*******************************
*		MAIN FUNCTION
*******************************/
int main(int argc, char *argv[]) {
	packet m;
	int rc;

	init();
	set_router();

	while (1) {
		rc = get_packet(&m);
		DIE(rc < 0, "Could not get the packet.");

		struct ether_header *eth_hdr = (struct ether_header*)m.payload;

		if (eth_hdr->ether_type == htons(ETH_P_IP)) {
			manage_ip_forwarding(&m, eth_hdr);
		} else if (eth_hdr->ether_type == htons(ETH_P_ARP)) {
			manage_arp_protocol(&m);
		}
	}
}
