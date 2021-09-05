Nume: Ghiculescu Alexandru
Grupa: 335CB

Tema 2

Implementare BLAS:

Pentru inceput copiez matricea B intr-o matrice auxiliara 
first, deoarece apelul functiei dtrmm va retine rezultatul
in first si B va trebui folosit ulterior. 

Apoi folosesc functia dtrmm pentru a inmulti A si first,
deoarece A este matrice triunghiulara. Dupa aceasta operatie,
in first am rezultatul A x B.

Copiez matricea A intr-o matrice auxiliara result, deoarece
urmeaza inca un apel al functie dtrmm. Inmultesc At cu result
si dupa aceasta operatie, in result am At x A.

In final folosesc functia dgemm care primeste 3 matrice, de
exemplu X Y Z si intoarce in Z rezultatul operatiei X * Y + Z
(unde coeficientii alfa si beta sunt egali cu 1). Aceasta
operatie este realizata cu parametrii first, B si result,
deoarece:
	first = A x B
	B = B 			 => result = A x B x Bt + At x A
	result = At x A

Prin parametrul CblasTrans (al treilea parametru din
apelul functie dgemm) precizez ca va fi folosit B transpus
pentru calculul de mai sus.

Implementare NEOPT:

Asemanator am folosit o matrice first in care am retinut
pentru inceput rezultatul operatiei A x B, realizata clasic
si tinand cont de faptul ca A este triunghiulara prin
ignorarea elementelor cu index al coloanei mai mic decat
cel al liniei.

Apoi am folosit o matrice result in care am realizat 
operatia first x Bt.

In final, am adaugat la result rezultatul operatiei At x A
unde am tinut cont de faptul ca matricea A este triunghiulara.
(Explicatia este aceeasi ca mai sus)

Implementare OPT:

Etapele sunt aceleasi ca in implementarea pentru NEOPT, cu
urmatoarele optimizari:
	- pentru prima operatie:
		- loop reorder: am folosit i-k-j in loc de i-j-k
		deoarece acceseaza matricele secvential sau 
		constant
		- plasarea constantelor din bucla intr-un registru:
		in bucla pentru J, A[i][j] este constanta si am
		folosit register a pentru a retine valoarea
		- optimizare acces la vectori prin folosirea
		de pointeri
	- pentru a doua operatie:
		- nu am mai folosit loop reorder deoarece a doua
		matrice este transpusa, deci accesul la date
		se face oricum secvential in i-j-k
		- plasarea constantelor din bucla intr-un registru
		- optimizare acces la vectori prin pointeri
	- pentru a treia operatie:
		- loop reorder: k-i-j in loc de i-j-k, deoarece
		aceeseaza matricele secvential sau constant
		- plasarea constantelor din bucla intr-un registru
		- optimizare acces la vectori prin pointeri

CACHEGRIND:

Instructions cache:
	- pentru toate implementarile, miss rate-ul este practic 0, deoarece
	numarul de instructiuni este relativ redus, si prin urmare
	toate instructiunile sunt gasite cache

Data cache:
	- se observa ca numarul de accesari/referinte la primul nivel de cache
	este cel mai mare la varianta neoptima, apoi optima (de 2x mai
	putine decat la neoptima), iar apoi blas (de 30x mai putine decat
	la neoptima). Miss rate-ul este asemanator la blas si la neoptima,
	iar la optim este de aprox 1.5x mai mic.
	- numarul de accesari/referinte la ultimul nivel de cache (L3/LL) 
	este cel mai mare la varianta neoptima, apoi la optima (de 3x mai
	putine decat la neoptima), iar apoi blas (de 30x mai putine decat
	la neoptima). Miss rate-ul este practic 0 in toate cele 3
	implementari.

Branches:
	- se observa ca numarul de misspredict-uri la branch-uri este asemanator
	pentru variantele optima si neoptima, iar pentru blas este de 8-10 ori
	mai mic, insa miss rate-ul este de 3 ori mai mare la blas decat la 
	variantele optima si neoptima.

Compararea performantei:
	- se observa din graficele generate ca cea mai buna varianta ca
	si performanta si deci ca si complexitate este varianta BLAS, care
	pentru N = 1200 are un timp de 30x ori mai bun decat varianta 
	neoptima si de 13-14x ori mai bun decat varianta optimizata.
	
	- comparand varianta optimizata cu varianta neoptimizata, observam
	ca cea dintai este de 2 ori mai buna pentru un N undeva mai mare 
	de 600. Pentru N mai mic, nu exista o diferenta semnificativa intre
	cele doua in ceea ce priveste performanta. In schimb, varianta BLAS
	este cu mult mai rapida si pentru un N mic (eg: N=400)
