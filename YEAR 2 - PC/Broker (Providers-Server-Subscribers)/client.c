#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "helpers.h"
#include <math.h>

fd_set read_fds;	/* File descriptors for select. */
fd_set tmp_fds;		/* Temporary file descriptor. 	*/
int fdmax;		/* Max value from read_fds.	*/
int sockfd;		/* Socket for receiving. 	*/
struct sockaddr_in serv_addr; /* Struct for client.	*/

int main(int argc, char *argv[]) {
	if (argc < 4) {
		usage(argv[0]);
	}

	set_client(argv[2], atoi(argv[3]));
	send_id(argv[1]);
	display_welcome(argv[1]);

	while (1) {
		tmp_fds = read_fds;

		int ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE (ret < 0, "select");

		if (FD_ISSET(sockfd, &tmp_fds)) {
			if (handle_receive() == EXIT) {
				break;
			}
		} else if (FD_ISSET(0, &tmp_fds)) {
			if (handle_client_stdin() == EXIT) {
				break;
			} 
		}
	}

	close(sockfd);

	return 0;
}

/*
*	Description: this function sets up the client main variables
*	(read_fds, tmp_fds, the socket, the serv_addr struct) and also makes
*	the connection and sets the fdmax variable.
*
*	Parameters: ip and the port for connection.
*/
void set_client(char *ip, short port) {

	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port 	 = htons(port);
	int ret = inet_aton(ip, &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton");

	ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "connect");

	FD_SET(sockfd, &read_fds);
	FD_SET(0, &read_fds);

	fdmax = sockfd;
}

/*
*	Description: this function sends client's id to the server.
*
*	Parameters: the client's id.
*/
void send_id(char *id) {
	tcp_msg sending_msg;
	memcpy(sending_msg.payload, id, ID_SIZE);
	int ret = send(sockfd, &sending_msg, sizeof(tcp_msg), 0);
	DIE(ret < 0, "send");
}

/*
*	Description: this function handles an udp message recieved from
*	the server. It calls other functions depending on the message type.
*	It also displays messages if the connection was closed by the server
*	or if the client tries to unsubscribe an non subscribed topic.
*
*	Return: 0(EXIT) if the client will close, 1 otherwise.
*/
int handle_receive() {
	udp_msg incoming_msg;
	int ret = recv(sockfd, &incoming_msg, sizeof(udp_msg), 0);

	if (ret == 0) {
		fprintf(stdout, "Server closed your connection.\n");
		fprintf(stdout, "Exiting...\n");
		return 0;
	}

	if (strncmp(incoming_msg.payload, "notopic", 7) == 0) {
		display_no_topic();
		return 1;
	}

	struct in_addr ip;
	ip.s_addr = incoming_msg.ip;

	if (incoming_msg.type == 0) {
		handle_type_0(incoming_msg, ip);
	} else if (incoming_msg.type == 1) {
		handle_type_1(incoming_msg, ip);
	} else if (incoming_msg.type == 2) {
		handle_type_2(incoming_msg, ip);		
	} else if (incoming_msg.type == 3) {
		handle_type_3(incoming_msg, ip);
	}

	return 1;
}

/*
*	Description: this function handles the process of a type 0 udp message.
*	It displays the udp provider's ip, port, topic and the processed value.
*
*	Parameters: the received udp message and a struct containing the udp  ip.
*/
void handle_type_0(udp_msg incoming_msg, struct in_addr udp_ip) {
	int sign 	= incoming_msg.payload[0];
	uint8_t oc1 = incoming_msg.payload[1];
	uint8_t oc2 = incoming_msg.payload[2];
	uint8_t oc3 = incoming_msg.payload[3];
	uint8_t oc4 = incoming_msg.payload[4];
	uint32_t no = (oc1 << 24) + (oc2 << 16) + (oc3 << 8) + oc4;

	no *= ((sign == 0) ? 1 : (-1));

	char buffer_topic[TOPIC_SIZE];

	memcpy(buffer_topic, incoming_msg.topic, TOPIC_SIZE);
	buffer_topic[TOPIC_SIZE] = '\0';

	fprintf(stdout, "%s:%d client_udp - %s - INT - %d\n",
		inet_ntoa(udp_ip),
		ntohs(incoming_msg.port),
		buffer_topic,
		no);
}

/*
*	Description: this function handles the process of a type 1 udp message.
*	It displays the udp provider's ip, port, topic and the processed value.
*
*	Parameters: the received udp message and a struct containing the udp  ip.
*/
void handle_type_1(udp_msg incoming_msg, struct in_addr udp_ip) {
	uint8_t oc1 = incoming_msg.payload[0];
	uint8_t oc2 = incoming_msg.payload[1];

	float aux = (oc1 << 8) + oc2;
	float no  = (float)(aux/100);

	char buffer_topic[TOPIC_SIZE];

	memcpy(buffer_topic, incoming_msg.topic, TOPIC_SIZE);
	buffer_topic[TOPIC_SIZE] = '\0';

	fprintf(stdout, "%s:%d client_udp - %s - SHORT_REAL - %.2f\n",
		inet_ntoa(udp_ip),
		ntohs(incoming_msg.port),
		buffer_topic,
		no);
}

/*
*	Description: this function handles the process of a type 2 udp message.
*	It displays the udp provider's ip, port, topic and the processed value.
*
*	Parameters: the received udp message and a struct containing the udp  ip.
*/
void handle_type_2(udp_msg incoming_msg, struct in_addr udp_ip) {
	int sign = incoming_msg.payload[0];

	uint8_t oc1 = incoming_msg.payload[1];
	uint8_t oc2 = incoming_msg.payload[2];
	uint8_t oc3 = incoming_msg.payload[3];
	uint8_t oc4 = incoming_msg.payload[4];
	uint32_t aux = (oc1 << 24) + (oc2 << 16) + (oc3 << 8) + oc4;

	uint8_t power = incoming_msg.payload[5];

	float no = (float)(aux/pow(10, power));

	no *= ((sign == 0) ? 1 : (-1));

	char buffer_topic[TOPIC_SIZE];

	memcpy(buffer_topic, incoming_msg.topic, TOPIC_SIZE);
	buffer_topic[TOPIC_SIZE] = '\0';

	fprintf(stdout, "%s:%d client_udp - %s - FLOAT - %.4f\n",
		inet_ntoa(udp_ip),
		ntohs(incoming_msg.port),
		buffer_topic,
		no);
}

/*
*	Description: this function handles the process of a type 3 udp message.
*	It displays the udp provider's ip, port, topic and the payload.
*
*	Parameters: the received udp message and a struct containing the udp  ip.
*/
void handle_type_3(udp_msg incoming_msg, struct in_addr udp_ip) {
	char buffer_payload[PAYLOAD_SIZE];
	char buffer_topic[TOPIC_SIZE];

	memcpy(buffer_payload, incoming_msg.payload, PAYLOAD_SIZE);
	memcpy(buffer_topic, incoming_msg.topic, TOPIC_SIZE);
	buffer_payload[PAYLOAD_SIZE] = '\0';
	buffer_topic[TOPIC_SIZE] = '\0';

	fprintf(stdout, "%s:%d client_udp - %s - STRING - %s\n",
		inet_ntoa(udp_ip),
		ntohs(incoming_msg.port),
		buffer_topic,
		buffer_payload);
}

/*
*	Description: this function handles client input from stdin.
*	It exits if the input is "exit" and it handles the input checks
*	for subscribe and unsubscribe commands before sending a tcp message
*	to the server.
*
*	Return: 0(EXIT) if the client will close, 1 otherwise.
*/
int handle_client_stdin() {
	char buffer[BUFLEN];

	memset(buffer, 0, BUFLEN);
	fgets(buffer, BUFLEN - 1, stdin);

	tcp_msg sending_msg;

	if (strncmp(buffer, "exit", 4) == 0) {
		fprintf(stdout, "Exiting...\n");

		memcpy(sending_msg.payload, "exit", 4);

		int ret = send(sockfd, &sending_msg, sizeof(tcp_msg), 0);
		DIE(ret < 0, "send");

		return 0;
	} else {
		char subscribe[15], topic[TOPIC_SIZE], SF[2];
		memset(subscribe, 0, 15);
		memset(topic, 0, TOPIC_SIZE);
		memset(SF, 0, SF_SIZE);
		sscanf(buffer, "%s %s %s", subscribe, topic, SF);

		if (strcmp(subscribe, "subscribe") == 0) {
			sending_msg.subscribe = 1;
		} else if (strcmp(subscribe, "unsubscribe") == 0) {
			sending_msg.subscribe = 0;
		} else {
			display_subscribe_error();
			return 1;
		}

		if (sending_msg.subscribe == 1) {
			if (strncmp(SF, "1", 1) == 0) {
				sending_msg.SF = 1;
			} else if (strncmp(SF, "0", 1) == 0) {
				sending_msg.SF = 0;
			} else {
				display_SF_error1();
				return 1;
			}
		} else if (sending_msg.subscribe == 0) {
			if (strlen(SF) > 0) {
				display_SF_error2();
				return 1;
			}
		}


		memcpy(sending_msg.payload, topic, TOPIC_SIZE);

		int ret = send(sockfd, &sending_msg, sizeof(tcp_msg), 0);
		DIE(ret < 0, "send");

		if (sending_msg.subscribe == 1) {
			fprintf(stdout, "Subscribed %s\n", sending_msg.payload);
		} else if (sending_msg.subscribe == 0) {
			fprintf(stdout, "Unsubscribed %s\n", sending_msg.payload);
		}

		return 1;
	}
}

/*
*	Description: this function display an error message if the words subscribe or
*	unsubscribe are misspelled.
*/
void display_subscribe_error() {
	fprintf(stderr, "\n--Input error----------------------------------------------------\n");
	fprintf(stderr, "| For subscribe/unsubscribe you need to use one of the following  \n");
	fprintf(stderr, "|  -> [subscribe] [topic] [SF] where SF = 1/0 only \n|  -> [unsubscribe] [topic] \n");
	fprintf(stderr, "-----------------------------------------------------------------\n\n");
}

/*
*	Description: this function display an error message if the the SF value is wrong or
*	there is no SF value.
*/
void display_SF_error1() {
	fprintf(stderr, "\n--Input error----------------------------------------------------\n");
	fprintf(stderr, "| For Store & Forward you need to use one of the following \n");
	fprintf(stderr, "|  -> [1]\n|  -> [0]\n");
	fprintf(stderr, "-----------------------------------------------------------------\n\n");
}

/*
*	Description: this function display an error message if the client tries to set a SF
*	value for unsubscribe.
*/
void display_SF_error2() {
	fprintf(stderr, "\n--Input error----------------------------------------------------\n");	
	fprintf(stderr, "| For unsubscribe command, you don't need a SF value.\n");
	fprintf(stderr, "-----------------------------------------------------------------\n\n");
}

/*
*	Description: this function display an welcome message and prints the instructions.
*/
void display_welcome(char *id) {
	fprintf(stdout, "-----------------------------------------------------------------\n");
	fprintf(stdout, "| Welcome, %s!\n", id);
	fprintf(stdout, "| Input format: [subscribe] [topic] [SF] or [unsubscribe] [topic]\n");
	fprintf(stdout, "| Type -exit- to close the application.\n");
	fprintf(stdout, "-----------------------------------------------------------------\n\n");
}

/*
*	Description: this function display an error message if the client tries to unsubscribe
*	a non subscribed topic.
*/
void display_no_topic() {
	fprintf(stderr, "\n--Unusbscribe error--------------------------------------------\n");
	fprintf(stderr, "| You are not subscribed to this topic.\n");
	fprintf(stdout, "-----------------------------------------------------------------\n\n");
}

/*
*	Description: this function display an error message if the parameters number is wrong.
*/
void usage(char *file) {
	fprintf(stderr, "Usage: %s server_address server_port\n", file);
	exit(0);
}
