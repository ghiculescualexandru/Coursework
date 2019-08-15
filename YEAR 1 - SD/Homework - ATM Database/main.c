/* GHICULESCU Alexandru-Ionut - 315CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int main(){		

	FILE *fp = fopen("input.in", "r");
	FILE *fo = fopen("output.out", "w");
	
	int nr_max_card;		/* The initial maximum number of cards.*/
	char action [1000];		/* A buffer where the operations is read. */
	int max_position = -1;	/* The maximum position a card reached. */
	int position = 0;		/* The current position. */
	int nr_cur_card = 0;	/* The current number of cards.*/

	fscanf(fp, "%d", &nr_max_card);
	/* Creating the database, the big linked list. */
	TLC LC = NULL;
	LC = create_list(nr_max_card);	
	while(fscanf(fp, "%s", action) == 1)
	{
		unsigned long long card_number, card_number_source, card_number_dest;
		char pin[5];
		char expiry_date [6];
		int cvv;
		int sum;

		switch (action_key(action)){

			case 1 : 

				fscanf(fp, "%llu", &card_number);
				fscanf(fp, "%s", pin);
				fscanf(fp, "%s", expiry_date);
				fscanf(fp, "%d", &cvv);
				/* Checking if the card exists. */
				if(check_card_exists(LC,card_number) == 0)
				{	
					fprintf(fo, "The card already exists\n");
					break;
				}
				/* Checking if the maximum number of cards has been reached.*/
				if(nr_cur_card == nr_max_card)
				{
					/* Extending the database if needed. */
					max_position = redim_database(&LC, nr_max_card);	
					nr_max_card *= 2;				
				}
				 
				position = (digit_sum(card_number)%nr_max_card);

				if(position > max_position)
					max_position = position;

				int error = add_card(LC,card_number,pin,expiry_date,cvv,position);
				if(error == 0)
					fprintf(stderr, "Eroare alocare\n");
				if(error == 1)
					nr_cur_card += 1;								

				break;

			case 2 : 				

				fscanf(fp, "%llu", &card_number);

				if(delete_card(LC,card_number,nr_max_card)==1)
					nr_cur_card -= 1;

				break;

			case 3 :

				fscanf(fp, "%llu", &card_number_source);
				fscanf(fp, "%llu", &card_number_dest);
				fscanf(fp, "%d", &sum);

				reverse_transaction(LC, card_number_source, card_number_dest, nr_max_card, sum, fo);

				break;

			case 4 :

				fscanf(fp, "%llu", &card_number);

				unblock_card(LC, card_number, nr_max_card);

				break;

			case 5 :

				if(fscanf(fp, "%llu", &card_number) == 1)
				{
					position = (digit_sum(card_number))%nr_max_card;
					show_one(LC, card_number, position, fo);					
				}
					
				else
					show_all(LC, max_position, fo);
			
				break;

			case 6 :

				fscanf(fp, "%llu", &card_number);
				fscanf(fp, "%s", pin);

				insert_card(LC, card_number, pin, nr_max_card, fo);

				break;

			case 7 :

				fscanf(fp, "%llu", &card_number);
				fscanf(fp, "%d", &sum);

				recharge(LC, card_number, nr_max_card, sum, fo);

				break;

			case 8 :

				fscanf(fp, "%llu", &card_number);
				fscanf(fp, "%d", &sum);

				cash_withdrawal(LC, card_number, nr_max_card, sum, fo);
				
				break;

			case 9 :

				fscanf(fp, "%llu", &card_number);

				balance_inquiry(LC, card_number, nr_max_card, fo);

				break;

			case 10 :  

				fscanf(fp, "%llu", &card_number_source);
				fscanf(fp, "%llu", &card_number_dest);
				fscanf(fp, "%d", &sum);

				transfer_funds(LC, card_number_source, card_number_dest, nr_max_card, sum, fo);

				break;

			case 11 :

				fscanf(fp, "%llu", &card_number);
				fscanf(fp, "%s", pin);

				pin_change(LC, card_number, nr_max_card, pin, fo);

				break;

			case 12 :

				fscanf(fp, "%llu", &card_number);
				cancel(LC, card_number, nr_max_card);

				break;

			default : break;
		}
	}	

	/* Free the memory used. */
	TLC aux_LC = LC;
	TLC aux2_LC = LC;

	for(; aux_LC; aux_LC = aux_LC->next)
	{
		
		TLSC aux_lsc = aux_LC->info;
		for(; aux_lsc; aux_lsc = aux_lsc->next)
			free_history(&(aux_lsc->info.history));
	}

	for(; aux2_LC; aux2_LC = aux2_LC->next)
		free_LSC(&(aux2_LC->info));

	free_list(&LC);

	fclose(fp);
	fclose(fo);

	return 1;
}

