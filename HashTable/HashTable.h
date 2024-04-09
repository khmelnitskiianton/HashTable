#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Hashing.h"
#include "HT_DLL_Types.h"

const size_t HT_MAX_CAPACITY = 50000;

typedef struct HT_Item {
    DLL_LinkList_t* DList;
    size_t          Amount;
} HT_Item_t;

typedef struct HashTable {
    HT_Item_t*  Items;
    size_t      Size; 
    size_t      (*HashFunction) (HT_Key_t); 
} HashTable_t;

int         HT_Ctor         (HashTable_t* myHashTable, size_t Size, size_t (*HashFunction) (HT_Key_t));
int         HT_Dtor         (HashTable_t* myHashTable);
int         HT_InitItem     (HT_Item_t* CurrentItem);
int         HT_CloseItem    (HT_Item_t* CurrentItem);
int         HT_Add          (HashTable_t* myHashTable, HT_Key_t Key, HT_Value_t Value);

inline __attribute__((always_inline)) DLL_Node_t* HT_Find(HashTable_t* myHashTable, HT_Key_t Key, HT_Value_t Value)
{
    size_t hash = myHashTable->HashFunction(Key);
    size_t index = hash % (myHashTable->Size);
    DLL_Elem_t NewValue = {hash, Key, Value, 1};
    //check for collusion
    DLL_Node_t* FindNode = DLL_Find(NewValue, myHashTable->Items[index].DList);
    return FindNode;
}

#endif