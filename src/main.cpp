#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../HashTable/DLinkList.h"
#include "../HashTable/HashTable.h"
#include "myassert.h"
#include "creator.h"

int WriteData(HashTable_t* HashTable, Information_t* InfoData);

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

    for (size_t i = 0; i < (InfoData.n_strings); i++)
    {
        HT_Add(&MainHashTable, (InfoData.string_buffer + i)->StartLine, 1);
    }

    //printf("%lu\n", MainHashTable.Items[74].Amount);
    //DLL_Node_t* CurrentNode = MainHashTable.Items[74].DList->Head;
    //while (CurrentNode)
    //{
    //    DLL_Elem_t Value = CurrentNode->Value;
    //    printf("%lu %s %lu %lu\n", Value.Hash, Value.Key, Value.Value, Value.Occurance);
    //    CurrentNode = CurrentNode->Next;
    //}

    WriteData(&MainHashTable, &InfoData);

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
            DLL_Elem_t Value = CurrentNode->Value;
            cases += Value.Value;
            CurrentNode = CurrentNode->Next;
        }
        fprintf(file_write, "%lu %lu\n", i, cases);
    }
    
    return 1;
}