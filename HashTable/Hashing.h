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

#define ASM_HASH

#ifndef ASM_HASH
size_t ElfHash      (const char* str, size_t length);
#else
extern "C" size_t ElfHash (const char* str, size_t length);
#endif

size_t Crc32Hash        (const char* str, size_t length);

#endif