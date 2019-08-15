# Homework - ATM Database Management #

The purpose of this homework was creating a database for an ATM (cash machine) using only generic singly linked lists. The main features of this ATM are: managing clients' requests, administrating their accounts and keeping a history of all transactions made.

The database structure: 
* A main list of cards (called LC), containing all cards, divided in sublists (LSC);
* Every element of the sublist contain the card information, including its history
(the card history is also a linked list).

Cards are added to the beggining of an LSC, depending on their card number and on the maximum number of cards(nr_max_card). The position is given by:  [sum of digits of card number] % nr_max_card. The maximum number of cards is also used if the current number of cards surpasses nr_max_card, in which case the 
maximum number of cards is doubled and the database is extended.