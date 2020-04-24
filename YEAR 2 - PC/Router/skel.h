#pragma once
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>
/* According to POSIX.1-2001, POSIX.1-2008 */
#include <sys/select.h>
/* ethheader */
#include <net/ethernet.h>
/* ether_header */
#include <arpa/inet.h>
/* icmphdr */
#include <netinet/ip_icmp.h>
/* arphdr */
#include <netinet/if_ether.h>
#include <net/if_arp.h>
#include <asm/byteorder.h>

/* 
 *Note that "buffer" should be at least the MTU size of the 
 * interface, eg 1500 bytes 
 */
#define MAX_LEN 1600
#define ROUTER_NUM_INTERFACES 4

#define DIE(condition, message) \
	do { \
		if ((condition)) { \
			fprintf(stderr, "[%d]: %s\n", __LINE__, (message)); \
			perror(""); \
			exit(1); \
		} \
	} while (0)

typedef struct {
	int len;
	char payload[MAX_LEN];
	int interface;
} packet;

extern int interfaces[ROUTER_NUM_INTERFACES];

int send_packet(int interface, packet *m);
int get_packet(packet *m);
char *get_interface_ip(int interface);
int get_interface_mac(int interface, uint8_t *mac);
void init();
void parse_arp_table();

/**
 * hwaddr_aton - Convert ASCII string to MAC address (colon-delimited format)
 * @txt: MAC address as a string (e.g., "00:11:22:33:44:55")
 * @addr: Buffer for the MAC address (ETH_ALEN = 6 bytes)
 * Returns: 0 on success, -1 on failure (e.g., string not a MAC address)
 */
int hwaddr_aton(const char *txt, uint8_t *addr);

/******************************************************************************/
/******************** MY DEFINITIONS ******************************************/
/******************************************************************************/
/************ Macro definitions ***********************************************/
#define SENT  1;
#define LOST  0;
#define HOLD -1;
/************ Strucutres definition *******************************************/
typedef struct route_table_entry {
	uint32_t prefix;
	uint32_t next_hop;
	uint32_t mask;
	int interface;
} table_entry;

typedef struct arp_entry {
	uint32_t ip;
	uint8_t mac[6];
} arp_entry;
/************ Managing different protocols ************************************/
int manage_arp_protocol(packet *m);
int manage_ip_forwarding(packet *m, struct ether_header *eth_hdr);
int manage_echo_request(packet *m, struct iphdr *ip_hdr);
/************ Managing the arp and routing tables *****************************/
void set_router();
void read_rtable();
void init_arp_table();
void update_arp_table(packet *m);
void sort_rtable();
void display_arp_table();
void display_rtable();
/************ Look up functiions **********************************************/
table_entry *lookup_rtable(__u32 ip) ;
arp_entry *get_arp_entry(__u32 ip);
/************ Managing the packets queue **************************************/
void send_waiting_packet();
void put_in_queue(packet *m);
/************ Managing requests and replies sents *****************************/
void send_icmp_reply(packet *m, uint8_t type) ;
void send_arp_request(uint32_t daddr, int ar_op);
void send_arp_reply(packet *m, int ar_op);
/************ Setting headers *************************************************/
void set_ether_header(packet *reply, packet *m) ;
void set_ethdr_arp(packet *reply, packet *m);
void set_ethdr_arp_request(packet *request);
void set_iphdr(packet *reply, packet *m);
void set_icmphdr(packet *reply, uint8_t type);
void set_arphdr_request(packet *request, uint32_t daddr, int ar_op) ;
void set_arphdr(packet *reply, packet *m, int ar_op) ;
/************ Setting requests and replies ************************************/
packet set_icmp_reply(packet *m); 
packet set_arp_reply(packet *m) ;
packet set_arp_request(uint32_t daddr) ;
/************ Sorting and searching  ******************************************/
void merge_sort(int low, int high) ;
void merge_halves(int l, int m, int r) ;
table_entry *binary_search(uint32_t dest_ip, int low, int high) ;
/************ Auxiliars *******************************************************/
uint32_t ip_to_dec(uint8_t o1, uint8_t o2, uint8_t o3, uint8_t o4);
uint16_t ip_checksum(void *vdata, size_t length);
int valid_dest(uint32_t dest_ip, int idx);
/******************************************************************************/
/******************************************************************************/