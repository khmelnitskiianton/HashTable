;========================================================================================
;                               MAIN NASM FOR HASH ON ASM                               |
;========================================================================================

global ElfHash
global Crc32Hash

section .text

;================

;ElfHash:
;        movsx   rdx, byte [rdi]
;        xor     eax, eax
;        test    dl, dl
;        je      .L5
;.L4:
;        sal     rax, 4
;        add     rdi, 1
;        add     rdx, rax
;        mov     rax, rdx
;        and     eax, 4026531840
;        mov     rcx, rax
;        shr     rcx, 24
;        xor     rcx, rdx
;        test    rax, rax
;        not     rax
;        cmovne  rdx, rcx
;        and     rax, rdx
;        movsx   rdx, byte[rdi]
;        test    dl, dl
;        jne     .L4
;        ret
;.L5:
;        ret

; unsigned int crc32c(const char * buf, unsigned long len)
; * buf - rdi
; len - rsi
;Crc32Hash:
;        mov     eax, -1
;        mov     edx, 4294967295
;        crc32   rax, QWORD [rdi]
;        crc32   rax, QWORD [rdi+8]
;        xor     rax, rdx
;        ret