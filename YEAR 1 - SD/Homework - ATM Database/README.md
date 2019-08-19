# Homework - ATM Database Management #

The main idea of this homework was building g a database for a cash machine using only generic singly linked lists. The ATM should be able to:
  * manage basic requests(checking balance inquiriy, withdraw money, etc);
  * administrate accounts(changing first PIN, blocking if necessary, etc);
  * keep a history of transactions and actions.

The database structure: 
* A main list of cards (called LC), containing all cards, divided in sublists (LSC);
* Every element of the sublist contain the card information, including its history
(the card history is also a linked list).

Cards are added to the beggining of an LSC, depending on their card number and on the maximum number of cards(nr_max_card). The position is given by:  [sum of digits of card number] % nr_max_card. The maximum number of cards is also used if the current number of cards surpasses nr_max_card, in which case the maximum number of cards is doubled and the database is extended.
