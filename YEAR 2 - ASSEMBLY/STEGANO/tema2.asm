; GHICULESCU ALEXANDRU-IONUT, 325 CB, TEMA 2 IOCLA

%include "include/io.inc"

bruteforce_singlebyte_xor:
    push ebp
    mov ebp, esp
    
    mov ecx, [img_height]       ; Numarul de linii.
    mov edx, [img_width]        ; Numarul de coloane.
    mov ebx, [img]              ; Pointer la imagine.   
                                ; Urmeaza un for in for.
    task1_outter_for:           ; For-ul pentru numarul de linii.
        cmp ecx, 0
        je task1_done_for
        
        dec ecx
        mov edx, [img_width]    ; De fiecare data refacem numarul de coloane.
        
    task1_inner_for1:           ; For-ul pentru numarul de coloane.
        cmp edx, 0
        je task1_outter_for
        
        push ecx                ; Pastrez pe stiva numarul de linii.
        mov ecx, 255            ; Pun in ECX numarul 255.
        jmp do_xor
        
    task1_inner_for2:  
        pop ecx                 ; Iau de pe stiva numarul de linii.
        add ebx, 4              ; Ma deplasez in imagine.
        dec edx                 ; Scad numarul de coloane.
        jmp task1_inner_for1
        
    do_xor:
        push ecx                ; Pun pe stiva numarul 255.
        
        xor ecx, [ebx]          ; Fac xor cu elementu la care ma aflu in imagine.
        cmp ecx, 'r'            ; Caut sa vad daca am gasit litera 'r'.
        je found_r              ; Daca am gasit 'r', am gasit cheia.
       
        pop ecx                 ; Scot numarul 255 de pe stiva.
        
        dec ecx                 ; Scad numarul.
        cmp ecx, 0              ; Vad daca am ajuns de la 255 la 0.
        je task1_inner_for2
        
        jmp do_xor              ; Incerc cu alt numar de la 255 in jos.
        
    found_r:
        pop ecx                 
        mov eax, ebx            ; Pun in EAX elementu la care ma aflu din imagine.
        
        add eax, 4              ; Merg la urmatorul element din imagine.
        push ecx                ; Pastrez cheia.
        xor ecx, [eax]          ; Fac xor cu cheia.
        cmp ecx, 'e'            ; Daca e litera 'e' continui.
        jne try_again
        
        pop ecx                 ; Scot de pe stiva cheia.
        push ecx                ; Pastrez cheia.
        
        add eax, 4              ; Continui sa vad daca am litera cautata.
        xor ecx, [eax]
        cmp ecx, 'v'
        jne try_again
        
        pop ecx
        push ecx
        
        add eax, 4              ; Continui sa vad daca am litera cautata.
        xor ecx, [eax]
        cmp ecx, 'i'
        jne try_again
        
        pop ecx
        push ecx
        
        add eax, 4              ; Continui sa vad daca am litera cautata.
        xor ecx, [eax]
        cmp ecx, 'e'
        jne try_again
        
        pop ecx
        push ecx
        
        add eax, 4              ; Continui sa vad daca am litera cautata.
        xor ecx, [eax]
        cmp ecx, 'n'
        jne try_again
        
        pop ecx
        push ecx
        
        add eax, 4              ; Continui sa vad daca am litera cautata.
        xor ecx, [eax]
        cmp ecx, 't'
        jne try_again
        
        jmp found_word          ; Daca am ajuns aici, inseamna ca am gasit cuvantul cautat.
        
    task1_done_for:
    
    try_again:
        pop ecx                 ; scot cheia de pe stiva.
        jmp task1_inner_for2    ; Incerc din nou pentru urmatorul element.
    
    found_word:   
        pop ebx                 ; Scot linia si cheia de pe stiva.
        pop eax
        push ebx                ; Pun inapoi linia si cheia pe stiva.
        push eax
    
    found_word_final:           ; Acesta este finalul primei functii .
        pop ecx                 ; In acest label salvez in eax si cheia si linia pentru a le returna.
        pop edx
        inc ecx
        mov eax, [img_height]
        sub eax, ecx
        mov ecx, eax       
        xor eax, eax
        mov eax, ecx
        shl eax, 16
        or eax, edx       

end_bruteforce_singlebyte_xor:
        leave
        ret
    
task2_singlebyte_xor:
    push ebp
    mov ebp, esp
    mov ecx, [ebp + 8]          ; Linia.
    mov edx, [ebp + 12]         ; Cheia veche.
        
    push ecx                    ; Pun pe stiva linia.
    push edx                    ; Pun pe stiva cheia veche.
        
    mov ecx, [img_height]
    mov edx, [img_width]
    mov ebx, [img]
        
    task2_outter_for:
        cmp ecx, 0
        je done_for_task2
            
        dec ecx
        mov edx, [img_width]    ; Refac mereu numarul de coloane.
      
    task2_inner_for:
        cmp edx, 0
        je task2_outter_for           
            
        pop eax                 ; Vechea cheie.
        push edx                ; Salvez pe stiva numarul de coloane.
        mov edx, [ebx]          ; Pun in edx elementul la care ma aflu din imagine.
        xor edx, eax            ; In edx am dupa primu xor.
            
        push eax                ; Salvez pe stiva registrele folosite pana acum.      
        push ecx
        push edx

        shl eax, 1              ; Formez noua cheie, shiftez la stanga cu 1, pentru a inmulti cu 2.
        add eax, 3              ; Adaug 3.
            
        xor edx, edx            ; Curat EDX pentru operatia de impartire.
        mov ecx, 5              ; In ECX pun 5.
        div ecx                 ; Impart la 5.
        sub eax, 4              ; Scad 4 si obtin noua cheie.
            
        pop edx                 ; Scot de pe stiva elementul din imagine la care ma aflu. 
        xor edx, eax            ; Fac xor cu noua cheie.
        pop ecx                 ; Scot de pe stiva registrele salvate anterior.         
        pop eax
           
        mov [ebx], edx          ; Modific elementul curent din imagine.      
        add ebx, 4              ; Merg la elementul umrator.
             
        pop edx                 ; Scot de pe stiva numarul de coloane.
        push eax                ; Pun vechea cheie pe stiva.
        dec edx
        jmp task2_inner_for
        
    done_for_task2:
        pop edx                 ; Scot de pe stiva vechea cheie.
        pop ecx                 ; Scot de pe stiva linia cautata.
        push edx                ; Salvez pe stiva vechea cheie.
            
        mov eax, ecx            ; Pun in EAX linia cautata.
        inc eax                 ; Ma duc la urmatoarea linie
            
        mov ebx, [img_width]
        mul ebx
        shl eax, 2              ; Am in EAX exact unde trebuie sa merg.
            
        mov ebx, [img]          ; In EBX pun imaginea.
        add ebx, eax            ; Ma deplasez la offset-ul dorit.
            
        ;C'est un proverbe francais.
        pop edx                 ; Scot vechea cheie de pe stiva.
                        
        mov eax, edx            ; Formez noua cheie.
        shl eax, 1
        add eax, 3
            
        xor edx, edx
        mov ecx, 5
        div ecx
        sub eax, 4
            
        mov edx, eax            ; Salvez in EDX noua cheie.
            
        mov ecx, 'C'            ; Pana la finalul functiei, codific fiecare litera cu noua cheie
        xor ecx, edx            ; si modific elementele imaginii in functie de asta.
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 39             ; ASCII pentru apostrof.
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 'e'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 's'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 't'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, ' '
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 'u'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 'n'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, ' '
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 'p'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
           
        mov ecx, 'r'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 'o'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 'v'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 'e'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 'r'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 'b'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 'e'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
                     
        mov ecx, ' '
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
           
        mov ecx, 'f'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
           
        mov ecx, 'r'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 'a'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 'n'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 'c'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 'a'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, 'i'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
      
        mov ecx, 's'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
            
        mov ecx, '.'
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
        
        mov ecx, 0
        xor ecx, edx          
        mov [ebx], ecx
        add ebx, 4
                         
end_task2_singlebyte_xor:
        leave
        ret    
    
morse_encrypt:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 16]         ; Offset.
    mov ebx, [ebp + 12]         ; String.
    mov ecx, [ebp + 8]          ; Img.
    
    mov edx, eax
    shl eax, 2
        
    add ecx, eax                ; Ma deplasez in imagine la offset-ul dorit.
        
    task3_start_for:            
        cmp byte [ebx], 0       ; Iterez prin string pana cand intalnesc 0.
        je end_morse_encrypt
            
        cmp byte [ebx], 'A'     ; Compar fiecare caracter al string-ului cu literele alfabetului,
        je A_letter             ; cu caracterul virgula si cu toate cifrele.
            
        cmp byte [ebx], 'B'
        je B_letter
            
        cmp byte [ebx], 'C'
        je C_letter
          
        cmp byte [ebx], 'D'
        je D_letter
            
        cmp byte [ebx], 'E'
        je E_letter
            
        cmp byte [ebx], 'F'
        je F_letter
            
        cmp byte [ebx], 'G'
        je G_letter
            
        cmp byte [ebx], 'H'
        je H_letter
            
        cmp byte [ebx], 'I'
        je I_letter
            
        cmp byte [ebx], 'J'
        je J_letter
            
        cmp byte [ebx], 'K'
        je K_letter
            
        cmp byte [ebx], 'L'
        je L_letter
            
        cmp byte [ebx], 'M'
        je M_letter
            
        cmp byte [ebx], 'N'
        je N_letter
            
        cmp byte [ebx], 'O'
        je O_letter
            
        cmp byte [ebx], 'P'
        je P_letter
            
        cmp byte [ebx], 'Q'
        je Q_letter
            
        cmp byte [ebx], 'R'
        je R_letter            
            
        cmp byte [ebx], 'S'
        je S_letter
            
        cmp byte [ebx], 'T'
        je T_letter  
                      
        cmp byte [ebx], 'U'
        je U_letter
          
        cmp byte [ebx], 'V'
        je V_letter
            
        cmp byte [ebx], 'W'
        je W_letter
            
        cmp byte [ebx], 'X'
        je X_letter   
                     
        cmp byte [ebx], 'Y'
        je Y_letter            
            
        cmp byte [ebx], 'Z'
        je Z_letter
            
        cmp byte [ebx], '1'
        je one_digit
            
        cmp byte [ebx], '2'
        je two_digit
            
        cmp byte [ebx], '3'
        je three_digit
          
        cmp byte [ebx], '4'
        je four_digit
            
        cmp byte [ebx], '5'
        je five_digit
            
        cmp byte [ebx], '6'
        je six_digit
            
        cmp byte [ebx], '7'
        je seven_digit            
            
        cmp byte [ebx], '8'
        je eight_digit
           
        cmp byte [ebx], '9'
        je nine_digit
            
        cmp byte [ebx], '0'
        je zero_digit
            
        cmp byte [ebx], ','
        je coma           
             
    task3_start_for_2: 
        mov byte [ecx], ' '
        add ecx, 4   
        mov dl, [ebx]
            
        inc ebx
        jmp task3_start_for
            
    coma:                       ; Codific fiecare element si il adaug in imagine
            mov byte [ecx], "-" ; simbol cu simbol.
            add ecx, 4
            mov byte [ecx], "-"
            add ecx, 4
            mov byte [ecx], "."
            add ecx, 4
            mov byte [ecx], "."
            add ecx, 4
            mov byte [ecx], "-"
            add ecx, 4
            mov byte [ecx], "-"
            add ecx, 4
            jmp task3_start_for_2
                
    A_letter:
            mov byte [ecx], "."
            add ecx, 4
            mov byte [ecx], "-"
            add ecx, 4
            jmp task3_start_for_2
    
     B_letter:
            mov byte [ecx], "-"
            add ecx, 4
            mov byte [ecx], "."
            add ecx, 4
            mov byte [ecx], "."
            add ecx, 4
            mov byte [ecx], "."
            add ecx, 4
            jmp task3_start_for_2
            
    C_letter:
            mov byte [ecx], "-"
            add ecx, 4
            mov byte [ecx], "."
            add ecx, 4
            mov byte [ecx], "-"
            add ecx, 4
            mov byte [ecx], "."
            add ecx, 4
            jmp task3_start_for_2
        
    D_letter:            
            mov byte [ecx], "-"
            add ecx, 4
            mov byte [ecx], "."
            add ecx, 4
            mov byte [ecx], "."
            add ecx, 4
            jmp task3_start_for_2
            
    E_letter:
            mov byte [ecx], "."
            add ecx, 4
            jmp task3_start_for_2
        
    F_letter:
            mov byte [ecx], "."
            add ecx, 4
            mov byte [ecx], "."
            add ecx, 4
            mov byte [ecx], "-"
            add ecx, 4
            mov byte [ecx], "."
            add ecx, 4
            jmp task3_start_for_2       
        
    G_letter:
            mov byte [ecx], "-"
            add ecx, 4
            mov byte [ecx], "-"
            add ecx, 4
            mov byte [ecx], "."
            add ecx, 4
            jmp task3_start_for_2 
            
    H_letter: 
            mov byte [ecx], "."
            add ecx, 4   
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4             
            jmp task3_start_for_2 
            
    I_letter: 
            mov byte [ecx], "."
            add ecx, 4   
            mov byte [ecx], "."
            add ecx, 4 
            jmp task3_start_for_2 
            
    J_letter: 
            mov byte [ecx], "."
            add ecx, 4   
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4 
            jmp task3_start_for_2 
            
    K_letter: 
            mov byte [ecx], "-"
            add ecx, 4   
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4            
            jmp task3_start_for_2 
            
    L_letter: 
            mov byte [ecx], "."
            add ecx, 4   
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4             
            jmp task3_start_for_2 
            
    M_letter: 
            mov byte [ecx], "-"
            add ecx, 4   
            mov byte [ecx], "-"
            add ecx, 4             
            jmp task3_start_for_2 

    N_letter: 
            mov byte [ecx], "-"
            add ecx, 4   
            mov byte [ecx], "."
            add ecx, 4            
            jmp task3_start_for_2 

    O_letter: 
            mov byte [ecx], "-"
            add ecx, 4   
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4             
            jmp task3_start_for_2 
            
    P_letter: 
            mov byte [ecx], "."
            add ecx, 4   
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4             
            jmp task3_start_for_2 
            
    Q_letter: 
            mov byte [ecx], "-"
            add ecx, 4   
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4          
            jmp task3_start_for_2 
            
    R_letter: 
            mov byte [ecx], "."
            add ecx, 4   
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4             
            jmp task3_start_for_2 

    S_letter: 
            mov byte [ecx], "."
            add ecx, 4   
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4             
            jmp task3_start_for_2 
            
    T_letter: 
            mov byte [ecx], "-"
            add ecx, 4             
            jmp task3_start_for_2 
            
    U_letter: 
            mov byte [ecx], "."
            add ecx, 4   
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4            
            jmp task3_start_for_2 
            
    V_letter: 
            mov byte [ecx], "."
            add ecx, 4   
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4             
            jmp task3_start_for_2 
            
    W_letter: 
            mov byte [ecx], "."
            add ecx, 4   
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4             
            jmp task3_start_for_2 
            
    X_letter: 
            mov byte [ecx], "-"
            add ecx, 4   
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4             
            jmp task3_start_for_2 

    Y_letter: 
            mov byte [ecx], "-"
            add ecx, 4   
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4             
            jmp task3_start_for_2 
            
    Z_letter: 
            mov byte [ecx], "-"
            add ecx, 4   
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4             
            jmp task3_start_for_2 
            
    one_digit: 
            mov byte [ecx], "."
            add ecx, 4   
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4             
            jmp task3_start_for_2 

    two_digit: 
            mov byte [ecx], "."
            add ecx, 4   
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4             
            jmp task3_start_for_2 
            
    three_digit: 
            mov byte [ecx], "."
            add ecx, 4   
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4             
            jmp task3_start_for_2 
            
    four_digit: 
            mov byte [ecx], "."
            add ecx, 4   
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4            
            jmp task3_start_for_2 
            
    five_digit: 
            mov byte [ecx], "."
            add ecx, 4   
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4             
            jmp task3_start_for_2 
        
    six_digit: 
            mov byte [ecx], "-"
            add ecx, 4   
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4            
            jmp task3_start_for_2 
            
    seven_digit: 
            mov byte [ecx], "-"
            add ecx, 4   
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4            
            jmp task3_start_for_2 
            
    eight_digit: 
            mov byte [ecx], "-"
            add ecx, 4   
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4            
            jmp task3_start_for_2 
            
    nine_digit: 
            mov byte [ecx], "-"
            add ecx, 4   
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "."
            add ecx, 4            
            jmp task3_start_for_2 
            
    zero_digit: 
            mov byte [ecx], "-"
            add ecx, 4   
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4 
            mov byte [ecx], "-"
            add ecx, 4            
            jmp task3_start_for_2             
                            
end_morse_encrypt:
    sub ecx, 4
    mov byte [ecx], 0               ; Adaug si caracterul 0 la final.
    
    push dword [img_height]         ; Afisez noua imagine.
    push dword [img_width]
    push dword [img]
    call print_image
    add esp, 12
        
    leave
    ret
        
lsb_encode:
        push ebp
        mov ebp, esp

        mov ecx, [ebp + 16]         ; Offset.
        mov ebx, [ebp + 12]         ; String.
        mov eax, [ebp + 8]          ; Image.
        
        shl ecx, 2
        
        add eax, ecx                ; Ma deplasez in imagine la offset-ul dorit.
        sub eax, 4
        
        task4_start_for:
            cmp byte [ebx], 0       ; Iterez prin string pana intalnesc caracterul 0.
            je end_lsb_encode           
            jmp encode
            
        task4_start_for_2:
            inc ebx
            jmp task4_start_for
        
        encode:
            xor edx, edx
            mov dl, [ebx]           ; Pun in EDX caracterul la care ma aflu in string.
           
            mov ecx, 8              ; In ECX tin un contor pentru a lua fiecare bit din caracter.
            jmp encode_letter
                        
            jmp task4_start_for_2   
           
        encode_letter:
            cmp ecx, 0              ; Ma opresc cand am terminat cu toti bitii caracterului.
            je task4_start_for_2
            dec ecx
            
            push edx                ; Salvez pe stiva litera.
            shr edx, cl             ; Shiftez la dreapta litera de 7/6/5.../0 ori.

            test dl, 1              ; Verific cel mai putin semnificativ bit.
            jnz for_one           
            jz for_zero

        encode_letter_2:       
            pop edx                 ; Scot de pe stiva litera.            
            jmp encode_letter

        for_one:
            or dword [eax], 1       ; Pun 1 pe cel mai putin semnificativ bit al elementului 
            add eax, 4              ; la care ma aflu in matrice si merg la urmatorul element.
            jmp encode_letter_2

        for_zero:
            shr byte [eax], 1       ; Pun 0 pe cel mai putin semnificativ bit al elementului
            shl byte [eax], 1       ; la care ma aflu in matrice si merg la urmatorul element.
            add eax, 4
            jmp encode_letter_2

end_lsb_encode:                
        mov ecx, 8                  ; Codific si caracterul 0, la fel ca mai sus.
        
        final_encode:
            cmp ecx, 0
            je return_lsb_encode
            mov edx, 0
            dec ecx
            shr edx, cl
            test dl, 1
            jnz final_one
            jz final_zero
   
        final_one:
            or dword [eax], 1   
            add eax, 4
            jmp final_encode
        
        final_zero:
            shr byte [eax], 1
            shl byte [eax], 1
            add eax, 4
            jmp final_encode
        
return_lsb_encode:
        push dword [img_height]     ; Afisez noua imagine.
        push dword [img_width]
        push dword [img]
        call print_image
        add esp, 12
        leave
        ret
        
lsb_decode:
        push ebp
        mov ebp, esp
        
        mov eax, [ebp + 8]          ; Image.
        mov ecx, [ebp + 12]         ; Offset.
        
        shl ecx, 2
        
        add eax, ecx                ; Ma deplasez in imagine la offset-ul dorit.
        sub eax, 4
        
        task5_start:
        xor ebx, ebx                ; Initializez litera cu null si contorul pentru biti cu 0.
        xor ecx, ecx
        
        task5_for_1:
            inc ecx                 
            mov edx, [eax]          ; Decodific prima litera
            and edx, 1
            shl ebx, 1
            or ebx, edx
            add eax, 4
            cmp ecx, 8              ; Merg de atatea ori cati biti am pentru un caracter.
            je task5_print
            
            jmp task5_for_1            
        
        task5_print:
            cmp ebx, 0              ; Verific daca litera obtinuta e 0, pentru a ma opri.
            je end_lsb_decode
        
            PRINT_CHAR bl           ; Afisez litera obtinuta.
            jmp task5_start
            
end_lsb_decode:
        leave
        ret
                       
extern atoi
extern printf
extern exit

; Functions to read/free/print the image.
; The image is passed in argv[1].
extern read_image
extern free_image
; void print_image(int* image, int width, int height);
extern print_image

; Get image's width and height.
; Store them in img_[width, height] variables.
extern get_image_width
extern get_image_height

section .data
	use_str db "Use with ./tema2 <task_num> [opt_arg1] [opt_arg2]", 10, 0

section .bss
    task:       resd 1
    img:        resd 1
    img_width:  resd 1
    img_height: resd 1
    
    img6:       resd 1
    imgW:       resd 1
    imgH:       resd 1
section .text
global main

main:
    ; Prologue
    ; Do not modify!
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]
    cmp eax, 1
    jne not_zero_param

    push use_str
    call printf
    add esp, 4

    push -1
    call exit

not_zero_param:
    ; We read the image. You can thank us later! :)
    ; You have it stored at img variable's address.
    mov eax, [ebp + 12]
    push DWORD[eax + 4]
    call read_image
    add esp, 4
    mov [img], eax

    ; We saved the image's dimensions in the variables below.
    call get_image_width
    mov [img_width], eax

    call get_image_height
    mov [img_height], eax

    ; Let's get the task number. It will be stored at task variable's address.
    mov eax, [ebp + 12]
    push DWORD[eax + 8]
    call atoi
    add esp, 4
    mov [task], eax

    ; There you go! Have fun! :D
    mov eax, [task]
    cmp eax, 1
    je solve_task1
    cmp eax, 2
    je solve_task2
    cmp eax, 3
    je solve_task3
    cmp eax, 4
    je solve_task4
    cmp eax, 5
    je solve_task5
    cmp eax, 6
    je solve_task6
    cmp eax, 7
    je solve_task7
    jmp done

solve_task1:

    push dword [img]
    call bruteforce_singlebyte_xor
    add esp, 4
    
    mov cx, ax
    shr eax, 16
    push ecx                        ; Salvez pe stiva cheia si linia
    push eax
    
    print_message1:      
                
        xor ebx, ebx
        mov ebx, [img_width]
        mul ebx
        shl eax, 2
       
        xor edx, edx
        mov edx, [img]
        add edx, eax                ; Ma duc la linia cautata.
       
        jmp final_print1
        
    final_print1:
        mov ecx, [img_width]
        pop ebx
        pop eax
        push eax
        push ebx
        jmp final_print_for1
           
    final_print_for1:
        cmp ecx, 0
        je end_task1
           
        push eax
        xor eax, [edx]
        cmp eax, 0
        je end_task1
        
        PRINT_CHAR al               ; Afisez caracter cu caracter
         
        pop eax
        add edx, 4                  ; Merg prin imagine
        
        dec ecx
        jmp final_print_for1
    
    end_task1:
        call bruteforce_singlebyte_xor
        mov cx, ax
        shr eax, 16                 ; Afisez linia si cheia pentru mesaj.
        NEWLINE
        PRINT_UDEC 4, ecx
        NEWLINE
        PRINT_UDEC 4, eax
        
        jmp done
        
solve_task2:
   
    call bruteforce_singlebyte_xor
    mov cx, ax                      ; Vechea cheie.
    shr eax, 16                     ; Linia.
    
    push ecx
    push eax    
    call task2_singlebyte_xor

    add esp, 8
    push dword [img_height]
    push dword [img_width]
    push dword [img]
    call print_image
    add esp, 12
    
    jmp done
    
solve_task3:

    mov edx, [ebp+12]
    push dword [edx+16]
    call atoi
    add esp, 4
    
    mov ecx, [ebp+12]
    mov ebx, [ecx+12]
  
    push eax
    push ebx
    push dword [img]
    call morse_encrypt
    add esp, 12

    jmp done
    
solve_task4:

    mov edx, [ebp+12]
    push dword [edx+16]
    call atoi
    add esp, 4
    
    mov ecx, [ebp+12]
    mov ebx, [ecx+12]
 
    push eax
    push ebx
    push dword [img]
    call lsb_encode
    add esp, 12
  
    jmp done
    
solve_task5:

    mov edx, [ebp + 12]
    push dword [edx + 12]
    call atoi
    add esp, 4
    
    push eax
    push dword [img]
    call lsb_decode
    add esp, 4
    
    jmp done
    
solve_task6:

    jmp done
    
solve_task7:
    
    jmp done

    ; Free the memory allocated for the image.
done:
    push DWORD[img]
    call free_image
    add esp, 4

    ; Epilogue
    ; Do not modify!
    xor eax, eax
    leave
    ret
    