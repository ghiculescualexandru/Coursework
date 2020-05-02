#ifndef _HELPERS_H
#define _HELPERS_H 1

#include <stdio.h>
#include <stdlib.h>

#define EXIT 				0		/* Code for conditional if's.			*/	
#define MAX_CLIENTS			10		/* Initial max number of clients.		*/
#define MAX_TOPIC	 		10		/* Initial max number of topics/client.	*/
#define BUFLEN				256		/* Max buffer size.						*/
#define PAYLOAD_SIZE		1500	/* Max payload size.					*/
#define TOPIC_SIZE			50		/* Max topic size.						*/
#define ID_SIZE				10		/* Max id size.							*/
#define SF_SIZE				2		/* Max SF size.							*/

/**********************************************************************/
/*
*	TCP Message Header. 
*	[subscribe | SF | payload]
*/
typedef struct tcp_msg {
	int subscribe; 		/* 1/0 = subscribe/unsubscribe			*/
	int SF; 			/* 1/0 = store&forward/NO store&forward */
	char payload[50];
} tcp_msg;
/**********************************************************************/
/*
*	UDP Message Header. 
*	[topic | type | payload | ip | port]
*/
typedef struct udp_msg {
	char topic[TOPIC_SIZE];
	uint8_t type;
	char payload[PAYLOAD_SIZE];
	unsigned long ip;
	unsigned short port;
} udp_msg;
/**********************************************************************/
/*
*	Topic Header. 
*	[subscribe | SF | name]
*/
typedef struct topic {
	int subscribe;		/* 1/0 = subscribe/unsubscribe			*/
	int SF; 			/* 1/0 = store&forward/NO store&forward */
	char name[ID_SIZE];
} topic;
/**********************************************************************/
struct queue;
typedef struct queue *queue;
typedef struct cell *list;
/*
*	TCP Client structure.
*/
typedef struct tcp_client {
	int topic_num;		/* Current number of topics. 			*/
	int max_topic_num;	/* Maximum number of topics(for realloc)*/
	int currsock;		/* Current socket (-1 if disconnected).	*/
	char id[ID_SIZE];	/* Client's id.							*/
	topic *topics;		/* Client's topics array. 				*/
	queue topics_queue;	/* Client's queue for topics with SF=1  */
} tcp_client;

/************ Managing Server *****************************************/
tcp_client* set_tcp_clients();
void set_server(int portno);
int  handle_server_stdin();
void handle_passive_tcp(tcp_client **clients);
void handle_passive_udp(tcp_client *clients);
void handle_active_tcp(tcp_client *clients);
void display_clients(tcp_client *clients);
/************ Managing TCP Client *************************************/
void set_client(char *ip, short port);
void send_id(char *id);
int  handle_client_stdin();
int  handle_receive();
void handle_type_0(udp_msg incoming_msg, struct in_addr udp_ip);
void handle_type_1(udp_msg incoming_msg, struct in_addr udp_ip);
void handle_type_2(udp_msg incoming_msg, struct in_addr udp_ip);
void handle_type_3(udp_msg incoming_msg, struct in_addr udp_ip);
/************ Managing messages display *******************************/
void display_subscribe_error();
void display_SF_error1();
void display_SF_error2();
void display_welcome();
void display_no_topic();
void display_server_connected();
/**********************************************************************/
struct cell {
  void *element;
  list next;
};

struct queue {
  list head;
  list tail;
};
/************ Managing list and queue *********************************/
list  cons(void *element, list l);
list  cdr_and_free(list l);
queue queue_create(void);
int   queue_empty(queue q);
void  queue_enq(queue q, void *element);
void *queue_deq(queue q);
/**********************************************************************/
void usage(char *file);
/*
 * Macro de verificare a erorilor
 * Exemplu:
 *     int fd = open(file_name, O_RDONLY);
 *     DIE(fd == -1, "open failed");
 */
#define DIE(assertion, call_description)	\
	do {									\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",	\
					__FILE__, __LINE__);	\
			perror(call_description);		\
			exit(EXIT_FAILURE);				\
		}									\
	} while(0)

#endif
/**********************************************************************/