/* GHICULESCU Alexandru-Ionut - 315CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct history{

	char * one_history;
	struct history * next;
	
} THistory, *TLHistory, **ALHistory;

typedef struct info{

	unsigned long long card_number;
	int cvv;
	unsigned int balance;
	char pin[5];
	char first_pin[5];
	char status[10];
	char expiry_date[6];
	TLHistory history;
	int pin_errors;	 
	
} info_card;

typedef struct LSC{

	info_card info;
	struct LSC * next;

} LSC, *TLSC, **ALSC;

typedef struct LC{

	TLSC info;
	struct LC * next;

} LC, *TLC, **ALC;

int action_key(char action[1000]);
int digit_sum(unsigned long long n);
TLC create_list(int nr_max_card);
TLSC allocation_LSC(
	unsigned long long card_number, 
	char pin[5], 
	char first_pin[5], 
	char expiry_date[6], 
	int cvv);
int check_card_exists(TLC list, unsigned long long card_number);
int add_card(
	TLC list, 
	unsigned long long card_number, 
	char pin[5], 
	char expiry_date[6], 
	int cvv, 
	int position);
void show_one(
	TLC LC, 
	unsigned long long card_number, 
	int position, 
	FILE * output);
void show_all(TLC list, int max_position, FILE * output);
int delete_card(TLC aL, unsigned long long card_number, int nr_max_card);
TLHistory allocation_history(char * e);
int insert_history(info_card *card, char * x);
TLSC search_card(TLC list, unsigned long long card_number, int nr_max_card);
int insert_card(
	TLC list, 
	unsigned long long card_number, 
	char pin[5], 
	int nr_max_card, 
	FILE * output);
int cancel(TLC list, unsigned long long card_number, int nr_max_card);
int unblock_card(TLC list, unsigned long long card_number, int nr_max_card);
int check_pin(char pin[5]);
int pin_change(
	TLC list, 
	unsigned long long card_number, 
	int nr_max_card, 
	char pin[5], 
	FILE * output);
int balance_inquiry(
	TLC list, 
	unsigned long long card_number, 
	int nr_max_card, 
	FILE * output);
int recharge(
	TLC list, 
	unsigned long long card_number, 
	int nr_max_card, 
	int sum, 
	FILE * output);
int cash_withdrawal(
	TLC list, 
	unsigned long long card_number, 
	int nr_max_card, 
	int sum, 
	FILE * output);
int transfer_funds(
	TLC list, 
	unsigned long long card1, 
	unsigned long long card2, 
	int nr_max_card, 
	int sum, 
	FILE * output);
int delete_history(ALHistory aL, char x[50]);
int reverse_transaction(
	TLC list, 
	unsigned long long card1, 
	unsigned long long card2, 
	int nr_max_card, 
	int sum, 
	FILE * output);
int redim_database(ALC aLC, int nr_max_card);
void free_history(ALHistory history_list);
void free_LSC(ALSC aLSC);
void free_list(ALC list);
