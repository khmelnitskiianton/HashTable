#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef char* HT_Elem_t;
const HT_Elem_t HT_POISON_ELEMENT = NULL;

const size_t HT_MAX_CAPACITY = 50000;

typedef struct HT_Item {
    DLL_LinkList_t* DList;
    size_t          Amount;
} HT_Item_t;

typedef struct HashTable {
    HT_Item_t* Items;
    size_t      Size; 
    size_t      (*HashFunction) (HT_Elem_t); 
} HashTable_t;

int HT_Ctor         (HashTable_t* myHashTable, size_t Size, size_t (*HashFunction) (HT_Elem_t));
int HT_Dtor         (HashTable_t* myHashTable);
int HT_InitItem     (HT_Item_t* CurrentItem);
int HT_CloseItem    (HT_Item_t* CurrentItem);

#endif