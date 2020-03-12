%include "includes/io.inc"
;%include "io.inc"
extern getAST
extern freeAST

%macro multiply_by_10 1 
    mov ecx, %1                 ; inmulteste cu '10' un registru
    shl %1, 3
    times 2 add %1, ecx
%endmacro

readNode:
    push ebp
    mov ebp, esp

    cmp dword [ebp + 8], 0      ; pentru a ma opri din parcurgerea arborelui, compar cu '0'
    je return
    
    mov eax, [ebp + 8]          ; mut in EAX parametrul primit pentru readNode
    mov ebx, [eax]              ; dereferentiere pointer
    
    cmp byte [ebx], '-'         ; verificare semn '-', cu label special pentru minus, restul semnelor duc catre acelasi label
    je minusOp                 
    cmp byte [ebx], '+'         ; verificare semn '+'
    je isOp
    cmp byte [ebx], '*'         ; verificare semn '*'
    je isOp
    cmp byte [ebx], '/'         ; verificare semn '/'
    je isOp    

    mov eax, [ebp + 8]          ; in cazul in care nodul este operand, dereferentiez informatia
    xor ebx, ebx
    mov edx, [eax]
    
    xor eax, eax
    xor ebx, ebx
  
convert:                        ; convertire de la string la integer
    cmp byte [edx], 0           ; ma opresc cand ajung la spatiu, adica '0'
    je return
    
    xor ebx, ebx
    mov bl, [edx]               ; convertire caracter cu caracter
    sub bl, '0'
    
    multiply_by_10 eax          ; pentru numere de doua sau mai multe cifre
    
    add eax, ebx
    inc edx
    jmp convert

minusOp:                        ; daca citesc minus, intai verific daca ce urmeaza este cifra sau spatiu
    cmp byte [ebx + 1], 0
    je isOp                     ; daca e  spatiu, minus este operator
    
    xor eax, eax
    xor edx, edx
    
convertMinus:                   ; altfel, este un operand negativ pe care il convertim din string la integer
    cmp byte [ebx + 1], 0       ; ma opresc la spatiu si merg la label-ul pentru iesirea din convertire
    je outOfConvertMinus
    
    xor edx, edx
    mov dl, [ebx + 1]
    sub dl, '0'
    
    multiply_by_10 eax
    
    add eax, edx
    
    inc ebx
    jmp convertMinus
    
outOfConvertMinus:              ; inmultesc cu -1 pentru a forma numarul negativ
    mov ecx, -1
    imul ecx    
    
    jmp return

isOp:
    push ebx                    ; pun pe stiva operandul, pentru a realiza operatia 
      
traverse:                       ; parcurgere recursiva a arborelui in preordine
    mov eax, [ebp + 8]    
    mov ebx, [eax + 4]          ; pun in ebx fiul stang al nodului curent
    push DWORD ebx
    call readNode
    add esp, 4
    
    push eax                    ; pun rezultatul functiei readNode de pe partea stanga pe stiva
    
    mov eax, [ebp + 8]
    mov ebx, [eax + 8]          ; pun in ebx fiul drept al nodului curent
    push DWORD ebx
    call readNode
    add esp, 4
    
    pop ebx                     ; extrag rezultatul functiei readNode pe partea dreapta de pe stiva
    pop edx                     ; extrag operandul de pe stiva
    
    cmp byte [edx], '+'         ; compar operandul si ma duc la label-ul potrivit
    je adunare
    
    cmp byte [edx], '-'
    je scadere
    
    cmp byte [edx], '*'
    je inmultire
    
    cmp byte [edx], '/'
    je impartire

adunare:                        ; adunarea celor doua numere
    add eax, ebx
    jmp return
    
scadere:                        ; scaderea celor doua numere
    sub ebx, eax
    mov eax, ebx
    jmp return
    
inmultire:                      ; inmultirea celor doua numere
    imul ebx
    jmp return
    
impartire:                      ; impartirea celor doua numere
    mov ecx, eax
    mov eax, ebx
    cdq                         ; extindere registru
    idiv ecx
    jmp return
    
return:    
    leave
    ret

section .bss
    root: resd 1                ; La aceasta adresa, scheletul stocheaza radacina arborelui
section .data    
section .text

global main
main:
    mov ebp, esp                ; for correct debugging
    
    push ebp
    mov ebp, esp
    
    call getAST                 ; Se citeste arborele si se scrie la adresa indicata mai sus
    mov [root], eax
   
    push eax
    call readNode
    add esp, 4
    PRINT_DEC 4, eax
    
    push dword [root]           ; Se elibereaza memoria alocata pentru arbore
    call freeAST

    
    xor eax, eax
    leave
    ret