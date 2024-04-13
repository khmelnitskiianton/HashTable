#ifndef HASHING_H
#define HASHING_H

#define ROL(x) (x << 1) | (x >> (sizeof(x) - 1))
#define ROR(x) (x >> 1) | (x << (sizeof(x) - 1))

size_t Hash0            (const char* str, size_t length);
size_t FirstLetterHash  (const char* str, size_t length);
size_t LengthHash       (const char* str, size_t length);
size_t SumHash          (const char* str, size_t length);
size_t SumLengthHash    (const char* str, size_t length);
size_t RORHash          (const char* str, size_t length);
size_t ROLHash          (const char* str, size_t length);
size_t ElfHash          (const char* str, size_t length);

#ifndef ASM_HASH
size_t Crc32Hash        (const char* str, size_t length);
#else
inline __attribute__((always_inline)) size_t Crc32Hash(const char *str, size_t length)
{
    size_t crc = -1;
    asm(
        ".intel_syntax noprefix                     \n"
        "   add     %[len], %[str]                  \n"
        "   mov     edx, 4294967295                 \n"   
        ".for_loop:                                 \n"  
        "   mov     %[crc], rdx                     \n"  
        "   add     %[str], 1                       \n" 
        "   crc32   %[crc], byte ptr [%[str]-1]         \n" 
        "   mov     rdx, %[crc]                     \n"
        "   cmp     %[len], %[str]                  \n"
        "   jne     .for_loop                       \n"        
        "   not     %[crc]                          \n" 
        ".att_syntax noprefix                         \n"
        : [crc] "=r" (crc)
        : [str] "r"  (str), [len] "r"  (length)
    );
    return crc;
}
#endif

#endif