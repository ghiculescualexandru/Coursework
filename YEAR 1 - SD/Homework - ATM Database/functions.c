/* GHICULESCU Alexandru-Ionut - 315CB */

#include "functions.h"

/**
 * Description: This function returns an integer depending on 
 * what operation (the string "action") needs to be done.
 **/
int action_key(char action[1000]){

	if(!strcmp(action, "add_card"))
		return 1;
	if(!strcmp(action,"delete_card"))
		return 2;
	if(!strcmp(action,"reverse_transaction"))
		return 3;
	if(!strcmp(action,"unblock_card"))
		return 4;
	if(!strcmp(action,"show"))
		return 5;
	if(!strcmp(action,"insert_card"))
		return 6;
	if(!strcmp(action,"recharge"))
		return 7;
	if(!strcmp(action,"cash_withdrawal"))
		return 8;
	if(!strcmp(action,"balance_inquiry"))
		return 9;
	if(!strcmp(action,"transfer_funds"))
		return 10;
	if(!strcmp(action,"pin_change"))
		return 11;
	if(!strcmp(action,"cancel"))
		return 12;

	return 0;
}

/**
 * Description: This function return the sum of a number's digits.
 **/
int digit_sum(unsigned long long n){ 
	
   int sum = 0; 
   while (n != 0) 
   { 
       sum = sum + n % 10; 
       n = n/10; 
   } 
   return sum; 
} 
 
/**
 * Description: This function allocates memory depending on the
 * maximum number of atm cards. It returns an "empty" linked list
 * (the database).
 **/
TLC create_list(int nr_max_card){

	TLC list = NULL;
	TLC end = NULL;

	int i;

	for(i = 0; i < nr_max_card; i++)
	{
		TLC aux = (TLC)calloc(sizeof(LC),1);
		if(!aux) return 0;

		aux->info = NULL;

		if(list == NULL)
			list = aux;

		else
			end->next = aux;

		end = aux;
	}

	return list;
}

/**
 * Description: This function allocates memory for a sublist, an atm
 * card, also retaining the card informations.
 **/
TLSC allocation_LSC(
	unsigned long long card_number, 
	char pin[5], 
	char first_pin[5], 
	char expiry_date[6], 
	int cvv){

	TLSC aux = (TLSC)malloc(sizeof(LSC));
	if(!aux) return NULL;
	
	aux->info.card_number = card_number;
	strcpy(aux->info.pin, pin);
	strcpy(aux->info.first_pin, first_pin);
	strcpy(aux->info.expiry_date,expiry_date);
	aux->info.cvv = cvv;

	aux->next = NULL;	

	return aux;
}

/**
 * Description: This function verify if one card already exists
 * checking by the card number if it was already inserted in 
 * any sublist from the database.
 * Input: The address for the TLC, the card number.
 * Output: 0 if found / 1 if not.
 **/
int check_card_exists(TLC list, unsigned long long card_number){
	TLC p = NULL;
	TLSC r = NULL;	

	for (p = list; p != NULL; p = p->next)
	{		
		r = p->info;

		for(; r != NULL; r = r->next)
			if(r->info.card_number == card_number)
				return 0;
	}

	return 1;
}

/**
 * Description: This function adds a card in the database. 
 * Input: The address for the TLC, card informations.
 * Output: 1 for succes / 0 for memory error.
 **/
int add_card(
	TLC list, 
	unsigned long long card_number, 
	char pin[5], 
	char expiry_date[6], 
	int cvv, 
	int position){

	int cnt = 0;

	TLSC aux_lsc = allocation_LSC(card_number, pin, pin, expiry_date, cvv);
	if(!aux_lsc) return 0;	

	for(cnt = 0; cnt < position; cnt++)
		list = list->next;

	aux_lsc->next = list->info;
	strcpy(aux_lsc->info.status, "NEW");
	aux_lsc->info.history = NULL;
	aux_lsc->info.pin_errors = 0;
	list->info = aux_lsc;
	aux_lsc->info.balance = 0;

	/*functia returneaza 1 pentru succes, 0 pentru esec la alocare*/
	return 1;
}

/**
 * Description: This function displays all the information about a card.
 **/
void show_one(
	TLC LC,
	unsigned long long card_number,
	int position,
	FILE * output){
	
	int i;

	for(i = 0; i < position; i++)
		LC = LC->next;

	TLSC r = LC->info;

	while(r)
	{
		if(r->info.card_number == card_number)
			break;

		r = r->next;
	}

	TLHistory t = r->info.history;

	fprintf(output,"(card number: %016llu, PIN: %s, expiry date: %s",
		r->info.card_number, r->info.pin, r->info.expiry_date);
	fprintf(output,", CVV: %03d, balance: %d, status: %s, history: [", 
		r->info.cvv, r->info.balance, r->info.status);

	while(t && t->next)
	{
		fprintf(output,"%s, ", t->one_history);
		t = t->next;
	}

	if(t)
	fprintf(output, "%s", t->one_history);

	fprintf(output,"])\n");
}

/**
 * Description: This function displays all the information about all
 * cards in the database.
 **/
void show_all(TLC list, int max_position, FILE * output){

	int i = 0 ;
	TLC p;
	TLSC r;

	for (p = list; p != NULL; p = p->next, i++)
	{		
		if(max_position == -1) break;		

		fprintf(output,"pos%d: [",i);

		r = p->info;

		for(; r != NULL; r = r->next)
		{
			fprintf(output,"\n(card number: %016llu, PIN: %s, expiry date: %s",
				r->info.card_number, r->info.pin, r->info.expiry_date);
			fprintf(output,", CVV: %03d, balance: %d, status: %s, history: [", 
				r->info.cvv, r->info.balance, r->info.status);

			TLHistory t = r->info.history;

			while(t && t->next)
			{
				fprintf(output, "%s, ", t->one_history);
				t = t->next;
			}

			if(t)
				fprintf(output, "%s", t->one_history);

			fprintf(output, "])");
		}

		if (p->info != NULL) fprintf(output, "\n");

		fprintf(output, "]\n");

		if(i == max_position) break;		
	}
}

/**
 * Description: This function deletes a card from the database.
 * Input: The address for the TLC, the card number, the max number.
 * Output: 1 for success / 0 the card is not found.
 **/
int delete_card(TLC aL, unsigned long long card_number, int nr_max_card){

	int position = (digit_sum(card_number))%nr_max_card;
	
	TLC aux_lc = aL;

	while(position)
	{
		position--;
		aux_lc = aux_lc->next;		
	}

	ALSC aux_lsc = &(aux_lc->info);

	TLSC ant, p;

	for(p=*aux_lsc, ant=NULL; p!=NULL; ant=p, p=p->next)
		if(p->info.card_number == card_number) break;

	if(p==NULL) return 0;

	if(ant==NULL) 
		*aux_lsc = p->next;
	else
		ant->next = p->next;

	free(p);

	return 1;
}

/**
 * Description: This function allocates memory for the history of 
 * one card from the database.
 **/
TLHistory allocation_history(char * e){        

	/*aloc memorie pentru un istoric si pentru un singur element din istoric*/
	TLHistory aux = (TLHistory)calloc(sizeof(THistory),1);  
	if(!aux) return 0;

	aux->one_history = (char*)calloc(sizeof(char),100);
		/*verific alocarea*/
	if(aux->one_history == NULL) return 0;

	if (aux)                           
	{ 
		/*pun in istoric un char* cu istoricul dorit*/
		strcpy(aux->one_history,e);
		/*leg celula la null*/                  
	    aux->next = NULL;
	}

	/*functia returneaza un element din istoric*/
	return aux;             
}

/**
 * Description: This function adds an element of history for 
 * one card from the database.
 **/
int insert_history(info_card *card, char * x){

	/*aloc memorie pentru un element de istoric si verific alocarea*/
	TLHistory aux = allocation_history(x);
	if(!aux) return 0;

	/*fac legaturile*/
	aux->next = card->history;
	card->history = aux;

	return 1;
}

/**
 * Description: This function returns the address of a needed card,
 * searching by the card number.
 **/
int i
TLSC search_card(TLC list, unsigned long long card_number, int nr_max_card){

	int position = (digit_sum(card_number))%nr_max_card;	
	TLC aux_lc = list;

	while(position)
	{
		position--;
		aux_lc = aux_lc->next;		
	}

	TLSC aux_lsc = aux_lc->info;

	while(aux_lsc)
	{		
		if(aux_lsc->info.card_number == card_number) 
			break;
		
		aux_lsc = aux_lsc->next;
	}

	return aux_lsc;
}

/**
 * Description: This function inserts a card in the atm.
 * Input: The address for the TLC, the card number and information.
 * Output: 1 success / 0 if the card status changed to LOCKED.
 **/
int insert_card(
	TLC list,
	unsigned long long card_number, 
	char pin[5], 
	int nr_max_card, 
	FILE * output){

	char history_op[50];

	
	TLSC aux_lsc = search_card(list, card_number, nr_max_card);	

	/* Checking if the card status is LOCKED. */
	if (strcmp(aux_lsc->info.status,"LOCKED") == 0)
	{
		fprintf(output,"The card is blocked. ");
		fprintf(output,"Please contact the administrator.\n");
		info_card *card = &(aux_lsc->info);
		sprintf(history_op, "(FAIL, insert_card %016llu %s)",
			card_number, pin);

		insert_history(card, history_op);

		return 1;
	}

	/* Checking the card PIN. */
	if(strcmp(aux_lsc->info.first_pin,pin) == 0)
	{
		fprintf(output, "You must change your PIN.\n");
		info_card *card = &(aux_lsc->info);
		sprintf(history_op, "(SUCCESS, insert_card %016llu %s)",
			card_number, pin);

		insert_history(card, history_op);

		aux_lsc->info.pin_errors = 0;

		return 1;
	}

	else if(strcmp(aux_lsc->info.pin,pin) == 0)
	{
		info_card *card = &(aux_lsc->info);
		sprintf(history_op, "(SUCCESS, insert_card %016llu %s)",
			card_number, pin);

		insert_history(card, history_op);
		aux_lsc->info.pin_errors = 0;

		return 1;		
	}
	
	else
	{
		info_card *card = &(aux_lsc->info);
		sprintf(history_op, "(FAIL, insert_card %016llu %s)",
			card_number, pin);

		insert_history(card, history_op);

		fprintf(output,"Invalid PIN\n");

		aux_lsc->info.pin_errors++;

		/* Locking the card if pin errors number reached 3. */
		if(aux_lsc->info.pin_errors == 3)
		{
			fprintf(output, "The card is blocked. ");
			fprintf(output, "Please contact the administrator.\n");
			strcpy(aux_lsc->info.status, "LOCKED");
		}

		return 0;
	} 

}

/**
 * Description: This function draws the card from the atm.
 **/
int cancel(TLC list, unsigned long long card_number, int nr_max_card){

	char history_op[50];

	TLSC aux_lsc = search_card(list, card_number, nr_max_card);
	info_card *card = &(aux_lsc->info);

	sprintf(history_op, "(SUCCESS, cancel %016llu)", card_number);
	insert_history(card, history_op);

	return 1;
}

/**
 * Description: This function changes the locked status of a card
 * into active status.
 **/
int unblock_card(TLC list, unsigned long long card_number, int nr_max_card){

	TLSC aux_lsc = search_card(list, card_number, nr_max_card);

	/* Reseting the pin errors number. */
	aux_lsc->info.pin_errors = 0;
	strcpy(aux_lsc->info.first_pin,"");
	/* The status is now active. */
	strcpy(aux_lsc->info.status, "ACTIVE");
	return 1;
}

/**
 * Description: This function checks if a PIN is valid.
 * This means it only has 4 digits and only digits.
 * Output: 1 for valid pin / 0 for invalid pin.
 **/
int check_pin(char pin[5]){

	if(strlen(pin) != 4) return 0;

	int ok = 0;
	int i;

	int length = strlen(pin);
	for(i = 0; i < length; i++)
	{
		if (pin[i] > '9' || pin[i] < '0')
			ok = 1;
	}

	if(ok == 1) return 0;
	else return 1;
}

/**
 * Description: This function change a card's PIN, if valid.
 * Input: The address for the TLC and card info.
 * Output: 1 for success / 0 card not found, invalid pin.
 **/
int pin_change(
	TLC list, 
	unsigned long long card_number, 
	int nr_max_card, 
	char pin[5], 
	FILE * output){

	char history_op[400];
	TLSC aux_lsc = search_card(list, card_number, nr_max_card);

	if(!check_pin(pin))
	{

		info_card *card = &(aux_lsc->info);
		sprintf(history_op, "(FAIL, pin_change %016llu %s)", card_number, pin);
		insert_history(card, history_op);
		fprintf(output, "Invalid PIN\n");

		return 0;
	}
	
	/* Modifying the card PIN. */
	strcpy(aux_lsc->info.pin,pin);
	/* Changing the card status into ACTIVE. */
	strcpy(aux_lsc->info.status,"ACTIVE");
	/* Delets the first card PIN. */
	strcpy(aux_lsc->info.first_pin,"");

	info_card *card = &(aux_lsc->info);
	
	sprintf(history_op, "(SUCCESS, pin_change %016llu %s)", card_number, pin);
	insert_history(card, history_op);
	return 1;

}

/**
 * Description: This function displays the balance inquiry
 * of a card from the database.
 **/
int balance_inquiry(
	TLC list, 
	unsigned long long card_number, 
	int nr_max_card, 
	FILE * output){

	TLSC aux_lsc = search_card(list, card_number, nr_max_card);
	char history_op[200];

	info_card *card = &(aux_lsc->info);
	sprintf(history_op, "(SUCCESS, balance_inquiry %016llu)", card_number);
	insert_history(card, history_op);
	fprintf(output, "%d\n", aux_lsc->info.balance);

	return 1;
}

/**
 * Description: This function recharges a card.
 * Input: The address for the TLC and card info.
 * Output: 1 for success / 0 card not found, or the
 * amount is not a multiple of 10.
 **/
int recharge(
	TLC list, 
	unsigned long long card_number,
	int nr_max_card, 
	int sum, 
	FILE * output){

	char history_op[200];
	TLSC aux_lsc = search_card(list, card_number, nr_max_card);

	/* The amount needs to be multiple of 10 case. */
	if(sum%10 != 0) 
	{
		fprintf(output, "The added amount must be multiple of 10\n");
		info_card *card = &(aux_lsc->info);

		sprintf(history_op, "(FAIL, recharge %016llu %d)", card_number, sum);
		insert_history(card, history_op);

		return 0;
	}

	info_card *card = &(aux_lsc->info);	
	aux_lsc->info.balance += sum;	

	fprintf(output, "%d\n", aux_lsc->info.balance);
	sprintf(history_op, "(SUCCESS, recharge %016llu %d)", card_number, sum);
	
	insert_history(card, history_op);

	return 1;
}

/**
 * Description: This function withdraw an amount from the card.
 * Input: The address for the TLC and card info.
 * Output: 1 for success / 0 card not found, or the
 * amount is not a multiple of 10.
 **/
int cash_withdrawal(
	TLC list, 
	unsigned long long card_number, 
	int nr_max_card, 
	int sum, FILE * output){

	char history_op[200];
	TLSC aux_lsc = search_card(list, card_number, nr_max_card);

	/* The amount needs to be multiple of 10 case. */
	if(sum%10 != 0) 
	{
		fprintf(output, "The requested amount must be multiple of 10\n");
		info_card *card = &(aux_lsc->info);

		sprintf(history_op, "(FAIL, cash_withdrawal %016llu %d)",
			card_number, sum);
		insert_history(card, history_op);

		return 0;
	}

	int cur_balance = aux_lsc->info.balance;
	/* The case in which the amount needed is bigger than the 
	amount the card has. */
	if(sum > cur_balance)
	{
		fprintf(output, "Insufficient funds\n");
		info_card *card = &(aux_lsc->info);

		sprintf(history_op, "(FAIL, cash_withdrawal %016llu %d)",
			card_number, sum);
		insert_history(card, history_op);

		return 0;
	}

	info_card *card = &(aux_lsc->info);
	aux_lsc->info.balance -= sum;

	fprintf(output, "%d\n", aux_lsc->info.balance);
	sprintf(history_op, "(SUCCESS, cash_withdrawal %016llu %d)",
		card_number, sum);
	insert_history(card, history_op);
	
	return 1;
}

/**
 * Description: This function transfers an amount from one card
 * to another.
 * Input: The address for the TLC and cards info.
 * Output: 1 for success / 0 card not found, or the
 * amount is not a multiple of 10.
 **/
int transfer_funds(
	TLC list, 
	unsigned long long card1, 
	unsigned long long card2, 
	int nr_max_card, 
	int sum, 
	FILE * output){

	char history_op1[200], history_op2[200];
	TLSC aux_lsc1 = search_card(list, card1, nr_max_card);
	TLSC aux_lsc2 = search_card(list, card2, nr_max_card);

	/* The amount needs to be multiple of 10 case. */
	if(sum%10 != 0) 
	{
		fprintf(output, "The transferred amount must be multiple of 10\n");
		info_card *card = &(aux_lsc1->info);

		sprintf(history_op1, "(FAIL, transfer_funds %016llu %016llu %d)",
			card1, card2, sum);
		insert_history(card, history_op1);

		return 0;
	}

	int cur_balance = aux_lsc1->info.balance;

	/* The case in which the amount needed is bigger than the 
	amount the card has. */
	if(cur_balance < sum)
	{
		fprintf(output, "Insufficient funds\n");
		info_card *card = &(aux_lsc1->info);

		sprintf(history_op1, "(FAIL, transfer_funds %016llu %016llu %d)",
			card1, card2, sum);
		insert_history(card, history_op1);

		return 0;
	}

	aux_lsc1->info.balance -= sum;
	aux_lsc2->info.balance += sum;

	info_card *card_source = &(aux_lsc1->info);
	info_card *card_dest = &(aux_lsc2->info);

	sprintf(history_op1, "(SUCCESS, transfer_funds %016llu %016llu %d)",
		card1, card2, sum);
	sprintf(history_op2, "(SUCCESS, transfer_funds %016llu %016llu %d)",
		card1, card2, sum);
	
	insert_history(card_source, history_op1);
	insert_history(card_dest, history_op2);
	
	fprintf(output, "%d\n", aux_lsc1->info.balance);

	return 1;
}

/**
 * Description: This function deletes an element of history from 
 * a card history.
 **/
int delete_history(ALHistory aL, char x[50]){

	TLHistory ant, p;
	for(p=*aL, ant=NULL; p!=NULL; ant=p, p=p->next)
		if(strcmp(p->one_history,x) == 0) break;

	if(p==NULL) return 0;

	if(ant==NULL) 
		*aL = p->next;
	else
		ant->next = p->next;

	free(p);

	return 1;
}

/**
 * Description: This function reverse a transaction made between
 * two cards, if possible.
 * Input: The address for the TLC and card info.
 * Output: 1 for success.
 **/
int reverse_transaction(
	TLC list, 
	unsigned long long card1, 
	unsigned long long card2, 
	int nr_max_card, 
	int sum, 
	FILE * output){

	char history_op1[200], history_op2[200];

	TLSC aux_lsc1 = search_card(list, card1, nr_max_card);
	TLSC aux_lsc2 = search_card(list, card2, nr_max_card);

	int cur_balance = aux_lsc2->info.balance;

	if(cur_balance < sum)
	{
		fprintf(output, "The transaction cannot be reversed\n");
	}

	else
	{	
		aux_lsc1->info.balance += sum;
		aux_lsc2->info.balance -= sum;

		info_card *card_source = &(aux_lsc1->info);
		info_card *card_dest = &(aux_lsc2->info);

		sprintf(history_op1,"(SUCCESS, reverse_transaction %016llu %016llu %d)",
			card1, card2, sum);
		sprintf(history_op2,"(SUCCESS, transfer_funds %016llu %016llu %d)",
			card1, card2, sum);

		/* Modyfing both cards histories. */
		insert_history(card_source, history_op1);
		delete_history(&(card_dest->history), history_op2);
	}

	return 1;
}

/**
 * Description: This function extends the database if the maximum
 * number of cards added reached a peak.
 **/
int redim_database(ALC aLC, int nr_max_card){
	
	int max_position = -1;
	TLC list = *aLC;
	TLC aux_LC = create_list(nr_max_card*2);

	for(; list; list = list->next)
	{
		while(list->info)
		{
			TLSC aux = list->info;
			list->info = list->info->next;
			int position = (digit_sum(aux->info.card_number))%(nr_max_card*2);

			if (position > max_position)
				max_position = position;

			TLC aux2_LC = aux_LC;

			while(position)
			{
				position--;
				aux2_LC = aux2_LC->next;
			}

			aux->next = aux2_LC->info;
			aux2_LC->info = aux;

		}
	}

	*aLC = aux_LC;
	return max_position;
}

/**
 * Description: Next 3 functions free the memory used.
 **/
void free_history(ALHistory history_list){

	TLHistory aux;
	while(*history_list)
	{
		aux = *history_list;
		*history_list = aux->next;
	
		free(aux->one_history);
		free(aux);
	}
}

void free_LSC(ALSC aLSC){

	TLSC aux;
	while(*aLSC)
	{
		aux = *aLSC;
		*aLSC = aux->next;
		free(aux);
	}
}

void free_list(ALC list){

	TLC aux;
	while(*list)
	{
    	aux = *list;
    	*list = aux->next;
    	free(aux);
  	}
}
