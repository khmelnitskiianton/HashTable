#ifndef HASHING_H
#define HASHING_H

#define ROL(x) (x << 1) | (x >> (sizeof(x) - 1))
#define ROR(x) (x >> 1) | (x << (sizeof(x) - 1))

size_t Hash0            (const char* str);
size_t FirstLetterHash  (const char* str);
size_t LengthHash       (const char* str);
size_t SumHash          (const char* str);
size_t SumLengthHash    (const char* str);
size_t RORHash          (const char* str);
size_t ROLHash          (const char* str);

#define ASM_HASH

#ifndef ASM_HASH
size_t ElfHash      (const char* str);
#else
extern "C" size_t ElfHash (const char* str);
#endif

size_t Crc32Hash        (const char* str);

#endif