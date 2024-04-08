#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "Hashing.h"

size_t Hash0 (const char* str)
{
    if (!str) return 0;
    return 0;
}

size_t FirstLetterHash (const char* str)
{
    if (!str) return 0;
    return  (size_t) str[0];
}

size_t LengthHash (const char* str)
{
    return strlen(str);
}

size_t SumHash (const char* str)
{
    size_t i = 0;
    size_t summ = 0;
    while (str[i] != '\0')
    {
        summ += (size_t) str[i];
        i++;
    }
    return summ;
}

size_t SumLengthHash (const char* str)
{
    size_t i = 0;
    size_t summ = 0;
    while (str[i] != '\0')
    {
        summ += (size_t) str[i];
        i++;
    }
    return summ / strlen(str);
}

size_t RORHash (const char* str)
{
    if (str[0] == '\0') return 0;
    size_t hash_val = *str;
    for (size_t i = 0; i < strlen(str); i++)
    {
        hash_val = (ROR(hash_val)) ^ ((size_t) str[i]);
    }
    return hash_val;
}

size_t ROLHash (const char* str)
{
    if (str[0] == '\0') return 0;
    size_t hash_val = *str;
    for (size_t i = 0; i < strlen(str); i++)
    {
        hash_val = (ROL(hash_val)) ^ ((size_t) str[i]);
    }
    return hash_val;
}

/*
  Name  : CRC-32
  Poly  : 0x04C11DB7    x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11
                       + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1
  Init  : 0xFFFFFFFF
  Revert: true
  XorOut: 0xFFFFFFFF
  Check : 0xCBF43926 ("123456789")
  MaxLen: 268 435 455 байт (2 147 483 647 бит) - обнаружение
   одинарных, двойных, пакетных и всех нечетных ошибок
*/
size_t Crc32Hash(const char *str)
{
    size_t len = strlen(str);
    size_t crc_table[256];
    size_t crc; int i, j;

    for (i = 0; i < 256; i++)
    {
        crc = i;
        for (j = 0; j < 8; j++)
            crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;

        crc_table[i] = crc;
    };

    crc = 0xFFFFFFFFUL;

    while (len--)
        crc = crc_table[(crc ^ *str++) & 0xFF] ^ (crc >> 8);

    return crc ^ 0xFFFFFFFFUL;
}

size_t MYHashFAQ6(const char* str)
{
    unsigned int hash = 0;
    size_t size = strlen(str);
    const char* z = str;

    for (; str < (z + size); str++)
    {
        hash += (unsigned char)(*str);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}