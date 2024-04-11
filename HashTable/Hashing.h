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

#define ASM_HASH 

#ifndef ASM_HASH
size_t Crc32Hash        (const char* str, size_t length);
#else
inline __attribute__((always_inline)) size_t Crc32Hash(const char *str, size_t length)
{
    size_t crc = -1;
    asm(
        ".intel_syntax noprefix                     \n"
        "   mov     %[crc], -1                      \n"
        "   mov     edx, 4294967295                 \n"        
        "   crc32   %[crc], QWORD PTR [%[str]]      \n"            
        "   crc32   %[crc], QWORD PTR [%[str]+8]    \n"            
        "   xor     rax, rdx                        \n"    
        ".att_syntax prefix                         \n"
        : [crc] "=r" (crc)
        : [str] "r" (str)
    );
    return crc;
}
#endif

#endif