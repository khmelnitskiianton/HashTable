#ifndef DLINKLIST_H
#define DLINKLIST_H

#include "HT_DLL_Types.h"

struct DLL_Node {
    DLL_Elem_t  Value;
    struct DLL_Node* Next;
    struct DLL_Node* Prev;
};

typedef struct DLL_Node DLL_Node_t;

typedef struct LinkList {
    DLL_Node_t* Head;
    DLL_Node_t* Tail;
} DLL_LinkList_t;

//===========================================================

int         DLL_Ctor        (DLL_LinkList_t* myLinkList);
int         DLL_Dtor        (DLL_LinkList_t* myLinkList);
int         DLL_InitNode    (DLL_Node_t* NewNode);

DLL_Node_t* DLL_PushFront   (DLL_Elem_t Value, DLL_LinkList_t* myLinkList);
DLL_Node_t* DLL_PushBack    (DLL_Elem_t Value, DLL_LinkList_t* myLinkList);
int         DLL_PopFront    (DLL_LinkList_t* myLinkList);
int         DLL_PopBack     (DLL_LinkList_t* myLinkList);

DLL_Node_t* DLL_Insert      (DLL_Node_t* InsertNode, DLL_Elem_t InsertValue);
int         DLL_Erase       (DLL_Node_t* EraseNode);

inline DLL_Node_t* DLL_Find (DLL_Elem_t Value, DLL_LinkList_t* myLinkList)
{
    DLL_Node_t* CurrentNode = myLinkList->Head;
    __m128i str1 = _mm_load_si128((const __m128i *) (Value.Key)); //load value ones
    while(CurrentNode != NULL)
    {
        //if (DLL_Compare(CurrentNode->Value, Value)) return CurrentNode; //strcmp() version
        
        __m128i str2 = _mm_load_si128((const __m128i *) (CurrentNode->Value.Key)); //load every value
        __m128i cmp  = _mm_cmpeq_epi64 (str1, str2);
        int result   = _mm_movemask_epi8 (cmp);
        if ((result == 0xFFFF) && (Value.Value == CurrentNode->Value.Value)) return CurrentNode;
        
        CurrentNode = CurrentNode->Next;
    }
    return NULL;
}

#endif
