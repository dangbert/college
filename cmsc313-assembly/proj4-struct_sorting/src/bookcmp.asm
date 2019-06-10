; Dan Engbert
; CMSC 313 Proj4
; Spring 2017

; byte offsets for fields in struct rec
%define AUTHOR_OFFSET    0
%define TITLE_OFFSET    21
%define SUBJECT_OFFSET  54
%define YEAR_OFFSET     68

        SECTION .bss

        SECTION .data

        SECTION .text
        extern book1
        extern book2
        global bookcmp

; sorts two struct books, based on year of publication, oldest year first
; for books in the same year, it subsorts by title, in alphabetic order
;
; returns integer value in the EAX register
;   -1 if book1 is BEFORE book2
;    0 if book1 is EQUAL TO book2
;    1 if book1 is AFTER book2

bookcmp: nop
        push    ebx
        push    ecx
        push    edx
        push    esi

        ; pointers to two struct books
        mov     ebx, book1
        mov     ecx, book2

        ; note: (sizeof(int)) == 4 on gl

        mov     eax, [ebx + YEAR_OFFSET]
        mov     edx, [ecx + YEAR_OFFSET]

c_year: nop
        ; compare years
        cmp     eax, edx
        jb      before
        ja      after

        ; the years must be equal so compare their titles alphabetically
        ; pointers to starts of each title
        add     ebx, TITLE_OFFSET
        add     ecx, TITLE_OFFSET

        mov     esi, 0                     ; count
loop:   nop
        mov     al, [ebx + esi]            ; current char of book1 title
        mov     dl, [ecx + esi]            ; current char of book2 title

        ; check if end of either string is reached
        cmp     al, 0
        je      a_null
        cmp     dl, 0
        je      d_null

        cmp     al, dl
        jb      before
        ja      after

        ; if chars are equal
        inc     esi
        jmp     loop


a_null: nop
        cmp     al, dl
        je      equal
        jmp     before


d_null: nop
        cmp     al, dl
        je      equal
        jmp     after


before: mov     eax, -1
        jmp end

equal:  mov     eax, 0
        jmp end

after: mov     eax, 1

end:    nop
        pop     esi
        pop     edx
        pop     ecx
        pop     ebx
        ret
