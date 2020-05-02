#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include "helpers.h"
#include <assert.h>

fd_set read_fds;	/* File descriptors for select. */
fd_set tmp_fds;		/* Temporary file descriptor. 	*/
int fdmax;			/* Max value from read_fds.		*/
int tcp_socket;		/* Passive tcp socket.			*/
int udp_socket;		/* Passive udp socket.			*/
int clients_num; 	/* Number of clients on server.	*/
int clients_max_size; /* Max number of clients.		*/
struct sockaddr_in serv_addr; /* Struct for serv.	*/

int main(int argc, char*argv[]) {
	if (argc < 2) {
		usage(argv[0]);
	}

	int ret;

	int portno = atoi(argv[1]);	/* Port number for server. */
	DIE(portno == 0, "atoi portno");

	set_server(portno);
	tcp_client* clients = set_tcp_clients();
	display_server_connected();

	while (1) {
		tmp_fds = read_fds;

		ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE (ret < 0, "select");

		if (FD_ISSET(0, &tmp_fds)) { // stdin
			if (handle_server_stdin() == EXIT) {
				break;
			} 
		} else if (FD_ISSET(tcp_socket, &tmp_fds)) { // tcp
			handle_passive_tcp(&clients);
		} else if (FD_ISSET(udp_socket, &tmp_fds)) { // udp
			handle_passive_udp(clients);
		} else { // e socket-ul unui client
			handle_active_tcp(clients);
		}
	}

	close(tcp_socket);
	close(udp_socket);

	for (int s = 0; s < clients_num; s++) {
  		close(clients[s].currsock);

  		free(clients[s].topics);

  		while(!queue_empty(clients[s].topics_queue)) {
  			udp_msg *temp = queue_deq(clients[s].topics_queue);
  			free(temp);
  		}

  		free(clients[s].topics_queue);

  	}

  	free(clients);

	return 0;
}

/*
*	Description: this function sets up the server's main variables
*	(read_fds, tmp_fds, passive and stdin sockets, the serv_addr struct).
*	It also makes all binds needed, the listen command and sets
*	the fdmax variable.
*
*	Parameters: the server's port number.
*/
void set_server(int portno) {

	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
	DIE(tcp_socket < 0, "tcp_socket");

	udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
	DIE(udp_socket < 0, "udp_socket");

	memset((char*) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family 		= AF_INET;
	serv_addr.sin_port 			= htons(portno);
	serv_addr.sin_addr.s_addr 	= INADDR_ANY; 

	int ret;

	ret = bind(tcp_socket, (struct sockaddr *) &serv_addr,
			sizeof(struct sockaddr));
	DIE(ret < 0, "bind tcp");

	ret = listen(tcp_socket, MAX_CLIENTS);
	DIE(ret < 0 , "listen");

	ret = bind(udp_socket, (struct sockaddr *) &serv_addr,
			sizeof(struct sockaddr));
	DIE(ret < 0, "bind udp");

	FD_SET(tcp_socket, &read_fds);
	FD_SET(udp_socket, &read_fds);
	FD_SET(0, &read_fds);

	fdmax = (tcp_socket > udp_socket) ? tcp_socket : udp_socket;
}

/*
*	Description: this function sets the number of clients on 0.
*	It sets the max number of clients and allocates memory for 
*	the array of clients.
*
*	Return: a pointer to the clients array.
*/
tcp_client* set_tcp_clients() {
	clients_num = 0;
	clients_max_size = MAX_CLIENTS;
	tcp_client* clients = (tcp_client*)malloc(MAX_CLIENTS * sizeof(tcp_client));
	return clients;
}

/*
*	Description: this function handles server input from stdin.
*	It exits if the input is "exit".
*
*	Return: 0(EXIT) if the client will close, 1 otherwise.
*/
int handle_server_stdin() {
	char buffer[BUFLEN];
	memset(buffer, 0, BUFLEN);
	fgets(buffer, BUFLEN - 1, stdin);

	if (strncmp(buffer, "exit", 4) == 0) {
		fprintf(stdout, "Server will shut down...\n");
		fprintf(stdout, "Server shut down.\n");

		return 0;
  	}

  	return 1;
}

/*
*	Description: this function handles communication with a passive tcp 
*	socket. If the new accepted socket is from a client connected before,
*	it empties the client's queue while sending the enqued messages.
*	Otherwise, if the client is completely new, it is added in the clients array.
*
*	Parameters: clients array.
*/
void handle_passive_tcp(tcp_client **clients) {
	struct sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);

	int newsock = accept(tcp_socket, (struct sockaddr *) &cli_addr, &clilen);
	DIE(newsock < 0, "accept new tcp socket");

	int ret;
	int flag = 1;

	ret = setsockopt(newsock, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));
	DIE(ret < 0, "setsockopt");

	FD_SET(newsock, &read_fds);

	fdmax = (newsock > fdmax) ? newsock : fdmax;

	tcp_msg incoming_msg;
	ret = recv(newsock, &incoming_msg, sizeof(tcp_msg), 0);
	DIE(ret < 0, "recv");
	
	char id[ID_SIZE];
	int ok = 1;
	memcpy(id, incoming_msg.payload, ID_SIZE);

	for (int i = 0; i < clients_num; i++) {
		if (strcmp((*clients)[i].id, id) == 0 && (*clients)[i].currsock == -1) {
			(*clients)[i].currsock = newsock;

			while (!queue_empty((*clients)[i].topics_queue)) {
				udp_msg *temp = queue_deq((*clients)[i].topics_queue);

				ret = send((*clients)[i].currsock, temp, sizeof(udp_msg), 0);
				DIE(ret < 0, "send");

				free(temp);
			}

			ok = 0;
		} else if (strcmp((*clients)[i].id, id) == 0 && (*clients)[i].currsock != -1) {
			close(newsock);
			FD_CLR(newsock, &read_fds);
			return;
		}
	}

	if (ok == 1) {
		if (clients_num >= clients_max_size) {
			tcp_client *temp = (tcp_client*)realloc(*clients,
								sizeof(tcp_client) * 2 * clients_max_size);
			*clients = temp;
			clients_max_size *= 2;
		}

		memcpy((*clients)[clients_num].id,incoming_msg.payload,ID_SIZE);
		(*clients)[clients_num].topics_queue = queue_create();
		(*clients)[clients_num].currsock = newsock;
		(*clients)[clients_num].max_topic_num = MAX_TOPIC;
		(*clients)[clients_num].topic_num = 0;
		(*clients)[clients_num].topics = (topic*)malloc(MAX_TOPIC * sizeof(topic));
		clients_num++;
	}
	(ok == 1) ? fprintf(stdout, "New") : fprintf(stdout, "Reconnected");
	fprintf(stdout, " client %s connected from %s:%d.\n", 
		incoming_msg.payload, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
}

/*
*	Description: this function handles communication with a passive udp
*	socket. It looks up the clients, searching for subscriptions.
*	If there are connected clients with subscriptions, it sends messages.
*	If there are disconnected clients with SF on, it enques messages.
*
*	Parameters: clients array.
*/
void handle_passive_udp(tcp_client *clients) {
	udp_msg incoming_msg;
	struct sockaddr_in provider;
	socklen_t length = sizeof(provider);

	memset(incoming_msg.topic, 0, TOPIC_SIZE);
	memset(incoming_msg.payload, 0, PAYLOAD_SIZE);

	int ret;

	ret = recvfrom(udp_socket, &incoming_msg, sizeof(udp_msg), 0,
				(struct sockaddr*) &provider, &length);

	incoming_msg.ip = provider.sin_addr.s_addr;
	incoming_msg.port = provider.sin_port;

	for (int s = 0; s < clients_num; s++) {
		tcp_client *client = &clients[s];

		for (int i = 0; i < client->topic_num; i++) {
			topic *topic = &client->topics[i];

			if (strcmp(topic->name, incoming_msg.topic) == 0 && topic->subscribe == 1) {
				if (client->currsock != -1) {
					ret = send(client->currsock, &incoming_msg,
							sizeof(udp_msg), 0);
					DIE(ret < 0, "send");
				} else if (client->currsock == -1 && topic->SF == 1) {
					udp_msg *temp = (udp_msg*)malloc(sizeof(udp_msg));

					memcpy(temp->topic, incoming_msg.topic, TOPIC_SIZE);
					temp->type = incoming_msg.type;
					memcpy(temp->payload, incoming_msg.payload, PAYLOAD_SIZE);
					temp->ip = incoming_msg.ip;
					temp->port = incoming_msg.port;

					queue_enq(client->topics_queue, temp);
				}
			}
		}
	}
}

/*
*	Description: this function handles communication with a active tcp
*	sockets. The server receives a message from an active tcp socket and
*	if the payload is exit, the client's socket is cleared. Otherwise,
*	it checks for subscribe/unsubscribe and updates the client's topics
*	array.
*
*	Parameters: clients array.
*/
void handle_active_tcp(tcp_client *clients) {
	for (int s = 0; s < clients_num; s++) {
		tcp_client *client = &clients[s];

		if (FD_ISSET(client->currsock, &tmp_fds)) {
			tcp_msg incoming_msg;
			int ret = recv(client->currsock, &incoming_msg, sizeof(tcp_msg), 0);
			DIE(ret < 0, "recv");

			if (strncmp(incoming_msg.payload, "exit", 4) == 0 || ret == 0) {
				fprintf(stdout, "Client %s disconnected.\n", client->id);

				client->currsock = -1;

				FD_CLR(client->currsock, &read_fds);
			} else if (incoming_msg.subscribe == 1){
				int topicExists = 0, index;
				int num = client->topic_num;

				for (int i = 0 ; i < num; i++) {
					topic *topic = &client->topics[i];
					if (strcmp(topic->name, incoming_msg.payload) == 0) {
						topicExists = 1;
						index = i;
					}
				}

				if (topicExists) {
					client->topics[index].subscribe = incoming_msg.subscribe;
					client->topics[index].SF 		= incoming_msg.SF;
				} else {
					if (client->topic_num >= client->max_topic_num) {
						topic *temp = (topic*)realloc(client->topics,
									client->max_topic_num * 2 * sizeof(topic));;
						client->topics = temp;
						client->max_topic_num *= 2;
					}

					client->topics[num].subscribe 	= incoming_msg.subscribe;
					client->topics[num].SF 			= incoming_msg.SF;
					memcpy(client->topics[num].name, incoming_msg.payload, TOPIC_SIZE);

					client->topic_num += 1;
				}
			} else if (incoming_msg.subscribe == 0) {
				int num = client->topic_num;
				int topicExists = 0;

				for (int i = 0 ; i < num; i++) {
					topic *topic = &client->topics[i];
					if (strcmp(topic->name, incoming_msg.payload) == 0) {
						topic->subscribe = 0;
						topicExists = 1;
					}
				}

				if (!topicExists) {
					udp_msg topic_err;
					memset(topic_err.payload, 0, PAYLOAD_SIZE);
					memcpy(topic_err.payload, "notopic", PAYLOAD_SIZE);
					int ret = send(clients[s].currsock, &topic_err,
								sizeof(udp_msg), 0);
					DIE(ret < 0, "send");
				}
			}
		}
	}
}

/*
*	Description: this function display a message for successful connection.
*/
void display_server_connected() {
	fprintf(stdout, "-----------------------------------------------------------------\n");
	fprintf(stdout, "| Server connected successfully.\n");
	fprintf(stdout, "-----------------------------------------------------------------\n");
}

/*
*	Description: this function display an error message if the parameters number is wrong.
*/
void usage(char *file) {
	fprintf(stderr, "Usage: %s server_port\n", file);
	exit(0);
}

/*
*	Description: this function displays all information about all clients.
*	Its pourpose is just for correct debugging.
*/
void display_clients(tcp_client *clients) {
	fprintf(stdout, "\n### debug ###\n\n");
	fprintf(stdout, "### Number of clients: %d\n\n", clients_num);
	for (int i = 0; i < clients_num; i++) {
		fprintf(stdout,"### Client %d, id: %s, currsock: %d, topic_num: %d, topics:\n", 
			i + 1, clients[i].id, clients[i].currsock, clients[i].topic_num);
		for (int j = 0; j < clients[i].topic_num; j++) {
			printf("### %s(nume), %d(sub), %d(SF)\n", clients[i].topics[j].name,
												clients[i].topics[j].subscribe,
												clients[i].topics[j].SF);
		}
		fprintf(stdout, "\n");
	}
	fprintf(stdout, "### debug ###\n");
}

/******** Functions for list and queue. **************************************/
list cons(void *element, list l) {
	list temp = malloc(sizeof(struct cell));
	temp->element = element;
	temp->next = l;
	return temp;
}

list cdr_and_free(list l) {
	list temp = l->next; 
	free(l);
	return temp;
}

queue queue_create(void) {
  queue q = malloc(sizeof(struct queue));
  q -> head = q -> tail = NULL;
  return q;
}

int queue_empty(queue q) {
  return q -> head == NULL;
}

void queue_enq(queue q, void *element) {
  if(queue_empty(q))
    q -> head = q->tail = cons(element, NULL);
  else
    {
      q->tail->next = cons(element, NULL);
      q->tail = q->tail->next;
    }
}

void *queue_deq(queue q) {
  assert(!queue_empty(q));
  {
    void *temp = q->head->element;
    q -> head = cdr_and_free(q->head);
    return temp;
  }
}
/*****************************************************************************/