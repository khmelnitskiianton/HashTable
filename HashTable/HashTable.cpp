#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "../DLinkedList/DLinkList.h"
#include "HashTable.h"

int HT_Ctor(HashTable_t* myHashTable, size_t Size, size_t (*HashFunction) (HT_Elem_t))
{
    if (Size > HT_MAX_CAPACITY) return 0;
    myHashTable->Size         = Size;
    myHashTable->HashFunction = HashFunction;
    myHashTable->Items = (HT_Item_t*) calloc (myHashTable->Size, sizeof(HT_Item_t));
    for (size_t i = 0; i < myHashTable->Size; i++)
    {
        HT_InitItem((myHashTable->Items)+i);
    }
    return 1;
}

int HT_Dtor(HashTable_t* myHashTable)
{
    for (size_t i = 0; i < myHashTable->Size; i++)
    {
        HT_CloseItem((myHashTable->Items) + i);
    }
    free(myHashTable->Items);
    return 1;
}

int HT_InitItem(HT_Item_t* CurrentItem)
{
    CurrentItem->Amount = 0;
    CurrentItem->DList = (DLL_LinkList_t*) calloc (1, sizeof(DLL_LinkList_t)); 
    DLL_Ctor(CurrentItem->DList);
    return 1;
}

int HT_CloseItem(HT_Item_t* CurrentItem)
{
    CurrentItem->Amount = 0;
    DLL_Dtor(CurrentItem->DList);
    free(CurrentItem->DList);
    return 1;
}

