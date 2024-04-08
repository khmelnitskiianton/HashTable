#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "DLinkList.h"
#include "HashTable.h"

int HT_Ctor(HashTable_t* myHashTable, size_t Size, size_t (*HashFunction) (HT_Key_t))
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
        HT_CloseItem(myHashTable->Items + i);
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

int HT_Add(HashTable_t* myHashTable, HT_Key_t Key, HT_Value_t Value)
{
    size_t hash = myHashTable->HashFunction(Key);
    size_t index = hash % (myHashTable->Size);
    DLL_Elem_t NewValue = {hash, Key, Value, 1};
    //check for collusion
    DLL_Node_t* CopyNode = DLL_Find(NewValue, myHashTable->Items[index].DList);
    if (CopyNode)
    {
        CopyNode->Value.Occurance++;
        return 1;
    }
    myHashTable->Items[index].Amount++;
    DLL_PushFront(NewValue, myHashTable->Items[index].DList);  
    return 1;
}

DLL_Node_t* HT_Find(HashTable_t* myHashTable, HT_Key_t Key, HT_Value_t Value)
{
    size_t hash = myHashTable->HashFunction(Key);
    size_t index = hash % (myHashTable->Size);
    DLL_Elem_t NewValue = {hash, Key, Value, 1};
    //check for collusion
    DLL_Node_t* FindNode = DLL_Find(NewValue, myHashTable->Items[index].DList);
    if (FindNode)
    {
        return FindNode;
    }
    return NULL;
}