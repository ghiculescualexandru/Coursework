Nume: Ghiculescu Alexandru
Grupa: 335CB

# Tema 1

# Organizare

Tema este implementarea problemei consumatorilor si producatorilor 
multipli, diferenta fata de problema clasica fiind faptul ca fiecare 
producator are propriul buffer in care produce, iar consumatorii 
consuma din unul dintre bufferele producatorilor. 

***Marketplace***

Pe langa parametrul primit din schelet pentru constructorul acestei clase,
(queue_size_per_producer) am adaugat urmatoarele campuri:
	- queues: cozile/bufferele producatorilor, sunt dictionare cu cheia
		    fiind id-ul producatorului si valoarea coada aferenta
		    acelui producator

    - carts: cosurile consumatorilor, sunt dictionare cu cheia fiind id-ul
    		cosului si valoarea fiind pereche (produs, id producator de
    		unde a fost extras acel produs la adaugarea in cos)

	- producers_no: numarul de produceri, util pentru a oferi id-uri la 
			inregistrarea unui producator

	- carts_no: numarul de cosuri, util pentru a oferi id-uri la generarea
			cosurilor

	- marketplace_lock: obiect de tip Lock folosit in momentul in care 
			un consumator adauga sau sterge din cos un produs, pentru a se
			evita accesul concurent la unul dintre bufferele producatorilor

	- producer_lock: obiect de tip Lock folosit in momentul inregistrarii
			unui producator, pentru a se evita asignarea aceluiasi ID mai
			multor producatori

	- cart_lock: obiect de tip Lock folosit in momentul generarii unui cos,
			pentru a se evita asignarea aceluiasi ID mai multor cosuri

In bufferul fiecarui producator pot fi introduse/produse noi produse doar daca
numarul curent de produse nu depaseste queue_size_per_producer. Acest maxim
poate fi depasit, in schimb, daca un consumator sterge din cosul sau un
produs al unui producator care are in acel moment bufferul la capacitate
maxima.

Cosul de cumparaturi are drept valori perechi (produs, id producator) tocmai
pentru a stii in bufferul carui producator sa fie readaugat produsul atunci
cand este sters dintr-un cos.

***Producer***

Producatorul primeste un ID iar apoi produce la infinit item-ele sale doar
daca bufferul sau nu este la capacitate maxima, altfel asteapta timpul
de republish

***Consumer***
Consumatorul primeste cate un ID pentru fiecare cos si realizeaza actiunile
pentru fiecare cos: add (daca se poate, altfel asteapta timpul de retry) si
remove. La finalul acestor actiuni plaseaza comanda.

# Implementare

Intregul enunt al temei a fost implementat, fara functionalitati extra si
fara dificultati deosebite intampinate.

# Git

https://github.com/ghiculescualexandru/tema1asc
