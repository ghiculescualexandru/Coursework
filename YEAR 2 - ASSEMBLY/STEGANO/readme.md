============================================================================================
## GHICULESCU ALEXANDRU-IONUT, 325 CB, TEMA 1 IOCLA ##			   
============================================================================================
        Voi explica codul mai sumar, deoarece in comentarii specific ce se intampla la fiecare
        instructiune. Voi explica cum realizez fiecare TASK conceptual, iar apoi voi detalia.

============================================================================================

## TASK 1 - Bruteforce pe XOR ##
                  
        Functia realizata intoarce in registrul EAX atat cheia si linia, in primii 32, 
        respectiv ultimii 32 de biti.
        
        In principiu, iterez prin intreaga imagine si incerc XOR cu fiecare numar de la 0 la 
        255, pana gasesc o combinatie care sa imi dea litera 'r'. In acel moment retin cheia
        si continui acelasi proces pana cand gasesc cuvantul 'revient'. In acel moment retin
        si linia la care ma aflu si salvez atat cheia cat si linia in EAX
        
        In main, ma duc la linia cautata si afisez caracter cu caracter decodificand cu cheia
        obtinuta anterior, si ma opresc cand intalnesc caracterul 0 sau cand s-a terminat 
        linia pe care se afla mesajul.
        
        Functia se afla la lin:3 unde se gasesc si comentariile care indica in detaliu intreg
        procesul, mai exact modul de iteratie, modul de bruteforce cu XOR pentru fiecare numar
        de la 0 la 255, si modul in care gasim linia in functie de gasirea cuvantului 'revient'.
        
============================================================================================
## TASK 2 - Criptare folosind XOR ##
                                    
        Functia realizata modifica imaginea primita ca parametru, pe urmatoarea linie
        de la care se afla mesajul primit la TASK 1 adaugand un nou mesaj dorit.
        
        In principiu, merg prin intreaga matrice si modific imaginea in functie de vechea 
        cheie obtinuta anterior si noua cheie obtinuta prin vehcea cheie. Apoi ma deplasez
        exact la urmatoarea linie de la cea la care am primit mesajul anterior.
        
        Dupa ce ajung pe linia dorita, scriu caracter cu caracter mesajul dorit codificand cu 
        noua cheie.
        
        Functia se afla la lin:173, unde se gasesc si comentariile care indica in detaliu
        ceea ce am rezumat anterior.
        
============================================================================================
## TASK 3 - Codul Morse ##
                                    
        Functia realizata afiseaza noua imagine modificata prin Codul Morse.
        
        In principiu, pentru inceput, ma deplasez la Offset-ul dorit, primit ca parametru. Apoi
        iterez prin string si pentru fiecare caracter merg sa modific corespunzator fiecare 
        element din matricea imaginii. Fiecare litera o codific cu mai multe simboluri pe care
        le aduug succesiv in matricea primita.
        
        La final adaug si caracterul 0 dupa mesajul criptat si afisez noua imagine criptata.      
        
============================================================================================
## TASK 4 - LSB ## 
                                    
        Functia modifica imaginea in functie de un string si un offset primit si metoda 
        modificarii celui mai nesemnificativ bit.
        
        In principiu, ma deplasez in imagine la Offset-ul dorit. Apoi iterez prin string-ul
        primit ca parametru pana cand intalnesc caracterul 0. Apoi pentru fiecare caracter,
        intr-o bucla shifez in functie de un contor de 7/6/5.../0 ori avand grija sa nu pierd
        caracterul dorit, salvandu-l pe stiva. De fiecare data cand shiftez, verific daca
        ultimul bit este 0 sau 1 prin 'test'. In cazul in care este 1, la elementul curent
        la care ma aflu in matrice, fac 'OR' cu 1, pentru a fi sigur ca pe cel mai 
        nesemnificativ bit din elementul curent se afla 1. In cazul in care este 0, pentru
        elementul curent la care ma aflu in matrice, shiftez succesiv la dreapta si la stanga 
        cu 1 pentru a forma 0 pe cel mai nesemnificativ bit. 
        
        La finalul functiei, in mod identic codific si ultimele elemente ale matricei in functie
        numarul 0, finalul stringului.
       
        Functia afiseaza matricea modificata.
        
============================================================================================
## TASK 5 - Decriptare LSB ##
                                    
        Functia afiseaza un mesaj criptat cu metoda prezentata la TASK 4.
        
        In principiu, ma deplasez in imagine la Offset-ul dorit. Apoi codific cate o litera
        mergand prin cate 8 elemente din matrice. La fiecare iteratie modific octetul pe care
        voi avea in final litera. 
        
        Dupa ce decodifc o litera, verific daca este 0, si atunci ma opresc. In caz contrar,
        creez un nou caracter null, un nou contor initializat cu 0 si iau din nou alte 8
        elemente din matrice si formez o noua litera.
        
        Fiecare litera este afisata in continuarea celeilalte in momentul in care se
        realizeaza 8 iteratii, pentru 8 biti.

