
----------------------------------------------------------------------------------------------
--		GHICULESCU ALEXANDRU-IONUT, 325 CB, TEMA 1 IOCLA			    --
----------------------------------------------------------------------------------------------
        Voi explica codul in ordine, de sus in jos, vorbind despre fiecare
        label si operatie.
----------------------------------------------------------------------------------------------
lin6:   Am utilizat un singur macro, pentru inmultirea unui registru cu 10. Dintr-un
        anume motiv, nu puteam sa folosesc "imul" cu un registru in care am pus anterior
        numarul 10, asadar am optat pentru o shiftare la stanga cu 3, adica inmultire cu 
        8, iar apoi doua adunari succesive.
----------------------------------------------------------------------------------------------
lin:12  "readNode" este functia ce mai intai verifica daca informatia din nod este operand
        sau operator. 
        Exista doua situatii: citeste semnul "minus" sau celelalte semne:
          - in cazul in care citeste minus, sare la label-ul "minusOp" (lin:52) unde verifica
        daca urmatorul caracter este spatiu sau nu. In cazul in care este spatiu, inseamna ca
        minus este operator, deci sare la label-ul "isOp". In cazul in care nu este spatiu,
        inseamna ca este un numar negativ si il converteste din string in integer in labe-ul
        "convertMinus"(lin:59). Voi explica cum este convertit un numar ulterior.
          - in cazul in care citeste altceva in afara de minus (*,/,+) sare direct la 
        label-ul "isOp".
        
        In cazul in care nu este operator, caracterul citit este inceputul unui string ce 
        reprezinta un numar.
        
        Conditia de oprire este citirea lui 0.
----------------------------------------------------------------------------------------------      
lin:38  String-ul este convertit in integer astfel:
          - fiecare caracter este transformat in integer, prin scaderea caracterului '0'.
          - rezultatul tinut in registrul EAX este inmultit cu 10 folosind macro-ul 
          "multiply_by_10" si i se adauga numarul transformat in integer.
          - se incrementeaza registrul EDX care pointeaza catre string-ul de convertit.
          
        Conditia de oprire este citirea lui 0.
----------------------------------------------------------------------------------------------
lin:80  "isOp" este label-ul care pune pe stiva operatorul (-,+,*,/) cand acesta este citit
        fie din "readNode", fie din "minusOp".
----------------------------------------------------------------------------------------------
lin:83  "traverse" parcurge arborele in preordine (root-left-right) astfel:
          - in registrul EAX pune nodul curent, ce se afla pe stiva, iar apoi in registrul
          EBX pune ce se afla la adresa nodului curent + 4, adica fiul stang al nodului.
          Pune pe stiva EBX, pentru a da ca parametru functiei "readNode" fiul stang.
          - acelasi lucru se repeta si pentru fiul drept, punand in registrul EBX ce se 
          afla la adresa nodului curent + 8.
          
        La lin:90 pune pe stiva EAX, adica rezultatul return dupa ce recursivitatea pe 
        partea stanga s-a incheiat si functia a ajuns la un operand sau la un rezultat
        de pe partea stanga.
          
        La lin:98 extrag de pe stiva rezultatul return de pe partea stanga in registrul
        EBX.
          
        La lin:99 extrag de pe stiva operatorul, apoi verific ce tip de operator e, 
        urmand un salt catre un label corespunzator.
          
        In functie de operatie, la label-ul specific utilizez EBX(rezultatul de pe
        partea stanga) si EAX, rezultatul return de pe partea dreapta.
----------------------------------------------------------------------------------------------
        Rezultatul final se afla in registrul EAX, deci in MAIN doar apelez functia
        "readNode", refac stiva si afisez rezultatul.
----------------------------------------------------------------------------------------------

