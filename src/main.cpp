#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <x86intrin.h>

#include "../HashTable/DLinkList.h"
#include "../HashTable/HashTable.h"
#include "myassert.h"
#include "creator.h"

int InsertData(HashTable_t* HashTable, Information_t* InfoData);
int StressTest(HashTable_t* HashTable, Information_t* InfoData);
int WriteData(HashTable_t* HashTable, Information_t* InfoData);

const size_t AMOUNT_TESTS = 256;
const unsigned long long clock_speed_spu = 3000000000;

int main(int argc, char** argv)
{ 
    if (argc < 5) 
    {
        printf("no args\n");
        return 0;
    }

    Information_t InfoData = {};

    InfoCtor(&InfoData, argv[1], argv[2]);

    HashTable_t MainHashTable = {};
    size_t ht_size = (size_t) atoi(argv[3]);
    switch (atoi(argv[4]))
    {
        case 1:
        HT_Ctor(&MainHashTable, ht_size, Hash0);    
        break;
        case 2:
        HT_Ctor(&MainHashTable, ht_size, FirstLetterHash);    
        break;
        case 3:
        HT_Ctor(&MainHashTable, ht_size, LengthHash);    
        break;
        case 4:
        HT_Ctor(&MainHashTable, ht_size, SumHash);    
        break;
        case 5:
        HT_Ctor(&MainHashTable, ht_size, SumLengthHash);    
        break;
        case 6:
        HT_Ctor(&MainHashTable, ht_size, RORHash);    
        break;
        case 7:
        HT_Ctor(&MainHashTable, ht_size, ROLHash);    
        break;
        case 8:
        HT_Ctor(&MainHashTable, ht_size, Crc32Hash);    
        break;
        case 9:
        HT_Ctor(&MainHashTable, ht_size, ElfHash);    
        break;
        default: return 0; break;
    }
    //upload table

    InsertData(&MainHashTable, &InfoData);
    
    WriteData(&MainHashTable, &InfoData);

#ifdef TESTING
    StressTest(&MainHashTable, &InfoData);
#endif

    InfoDtor(&InfoData);
    HT_Dtor(&MainHashTable);
    return 0;
}

int WriteData(HashTable_t* HashTable, Information_t* InfoData)
{
    FILE* file_write = InfoData->file_write;
    size_t amount_of_keys = 0;
    
    for (size_t i = 0; i < (HashTable->Size); i++)
    {
        amount_of_keys += HashTable->Items[i].Amount;
    }

    fprintf(file_write, "%lu\n", amount_of_keys);
    for (size_t i = 0; i < (HashTable->Size); i++)
    {
        size_t cases = 0;
        amount_of_keys += HashTable->Items[i].Amount;
        DLL_Node_t* CurrentNode = HashTable->Items[i].DList->Head;
        while (CurrentNode)
        {
            cases++;
            CurrentNode = CurrentNode->Next;
        }
        fprintf(file_write, "%lu %lu\n", i, cases);
    }
    
    return 1;
}

int InsertData(HashTable_t* HashTable, Information_t* InfoData)
{
    for (size_t i = 0; i < (InfoData->n_strings); i++)
    {
        HT_Add(HashTable, (InfoData->string_buffer+i)->StartLine, (InfoData->string_buffer+i)->Length);
    }
    return 1;
}

int StressTest(HashTable_t* HashTable, Information_t* InfoData)
{
    size_t t1 = __rdtsc();
    for (size_t i = 0; i < AMOUNT_TESTS; i++)
    {
        for (size_t j = 0; j < (InfoData->n_strings); j++)
        {
            DLL_Node_t* FindNode = HT_Find(HashTable, (InfoData->string_buffer+j)->StartLine, (InfoData->string_buffer+i)->Length);
            if (FindNode) printf("%c", FindNode->Value.Key[0]);
        }
    }
    size_t t2 = __rdtsc();
    size_t time = (t2 - t1);
    printf("\n>>> Time of StressTest: %lu\n", time);
    return 1;
}