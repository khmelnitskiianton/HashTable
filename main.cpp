#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "./DLinkedList/DLinkList.h"
#include "./HashTable/HashTable.h"

size_t hash1 (char* str);

int main(int argc, char** argv)
{
    HashTable_t MainHashTable = {};
    HT_Ctor(&MainHashTable, 10, hash1);
    HT_Dtor(&MainHashTable);
    return 0;
}

size_t hash1 (char* str)
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

