#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <smmintrin.h>

#include "DLinkList.h"
#include <assert.h>

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define RESET "\033[1;0m"

#define DEBUG 1

enum CODES_OF_ERRORS {
    BAD_POINTER_PASSED_IN_FUNC  = 1,
    POP_FRONT_BUT_HEAD_NULL     = 2,
    POP_FRONT_BUT_TAIL_NULL     = 3,
    CRASH_IN_CALLOC_OF_NODE     = 4,
    ERASE_HEAD_OR_TALE          = 5,
};

#ifdef DEBUG
#define MYASSERT(check, error_code, ending)                                                                         \
        do                                                                                                          \
        {                                                                                                           \
            if (!(check))                                                                                           \
            {                                                                                                       \
                printf(MAGENTA ">>>>> Error in code! Error code: %s <<<<<\n", #error_code);                         \
                printf("In File: %s, In Line: %d, In Function: %s\n", __FILE__, __LINE__,__PRETTY_FUNCTION__);      \
                printf("In this condition:\t\"%s\"\n\n" RESET, #check);                                             \
                ending;                                                                                             \
            }                                                                                                       \
        }                                                                                                           \
        while(0);
#else   
#define MYASSERT(check, error_code, ending)
#endif

//=======================================================================================

int DLL_Ctor (DLL_LinkList_t* myLinkList)
{
    MYASSERT(myLinkList, BAD_POINTER_PASSED_IN_FUNC, return 0)

    myLinkList->Head = NULL;
    myLinkList->Tail = NULL;

    return 1;
}

int DLL_Dtor (DLL_LinkList_t* myLinkList)
{
    MYASSERT(myLinkList, BAD_POINTER_PASSED_IN_FUNC, return 0)

    if (!(myLinkList->Head)) return 1;

    while(myLinkList->Head)
    {
        DLL_PopFront(myLinkList);
    }

    return 1;
}

int DLL_InitNode  (DLL_Node_t* NewNode)
{
    MYASSERT(NewNode, BAD_POINTER_PASSED_IN_FUNC, return 0)

    NewNode->Next = NULL;
    NewNode->Prev = NULL;
    NewNode->Value = POISON_ELEMENT;

    return 1;
}

DLL_Node_t* DLL_PushFront (DLL_Elem_t Value, DLL_LinkList_t* myLinkList)
{
    MYASSERT(myLinkList, BAD_POINTER_PASSED_IN_FUNC, return 0)
    DLL_Node_t* NewNode = (DLL_Node_t*) calloc (16, sizeof (DLL_Node_t));
    MYASSERT(NewNode, CRASH_IN_CALLOC_OF_NODE, return 0)
    
    DLL_InitNode(NewNode);

    if ((myLinkList->Head == NULL) && (myLinkList->Tail == NULL))
    {
        myLinkList->Head = NewNode;
        myLinkList->Tail = NewNode;
        NewNode->Value = Value;
        return NewNode;
    }

    DLL_Node_t* OldHead = myLinkList->Head;
    myLinkList->Head = NewNode;
    
    NewNode->Value = Value;
    NewNode->Prev = NULL;
    NewNode->Next = OldHead;

    OldHead->Prev = NewNode;

    return NewNode;
}

DLL_Node_t* DLL_PushBack  (DLL_Elem_t Value, DLL_LinkList_t* myLinkList)
{
    MYASSERT(myLinkList, BAD_POINTER_PASSED_IN_FUNC, return 0)
    DLL_Node_t* NewNode = (DLL_Node_t*) calloc (1, sizeof (DLL_Node_t));
    MYASSERT(NewNode, CRASH_IN_CALLOC_OF_NODE, return 0)

    DLL_InitNode(NewNode);

    if ((myLinkList->Head == NULL) && (myLinkList->Tail == NULL))
    {
        myLinkList->Head = NewNode;
        myLinkList->Tail = NewNode;
        NewNode->Value = Value;
        return NULL;
    }

    DLL_Node_t* OldTail = myLinkList->Tail;
    myLinkList->Tail = NewNode;
    
    NewNode->Value = Value;
    NewNode->Next = NULL;
    NewNode->Prev = OldTail;

    OldTail->Next = NewNode;

    return NewNode;
}

int DLL_PopFront  (DLL_LinkList_t* myLinkList)
{
    MYASSERT(myLinkList, BAD_POINTER_PASSED_IN_FUNC, return 0)

    if (!(myLinkList->Head)) return 1;

    DLL_Node_t* PopNode = myLinkList->Head;
    myLinkList->Head = PopNode->Next;
    if (myLinkList->Head) (myLinkList->Head)->Prev = NULL;

    free(PopNode);
    
    return 1;
}

int DLL_PopBack   (DLL_LinkList_t* myLinkList)
{
    MYASSERT(myLinkList, BAD_POINTER_PASSED_IN_FUNC, return 0)
    MYASSERT(myLinkList->Tail, POP_FRONT_BUT_TAIL_NULL, return 0)

    DLL_Node_t* PopNode = myLinkList->Tail;
    myLinkList->Tail = PopNode->Prev;
    (myLinkList->Head)->Next = NULL;

    free(PopNode);
    
    return 1;
}

DLL_Node_t* DLL_Insert (DLL_Node_t* InsertNode, DLL_Elem_t InsertValue)
{
    MYASSERT(InsertNode,  BAD_POINTER_PASSED_IN_FUNC, return 0)

    DLL_Node_t* NewNode = (DLL_Node_t*) calloc (1, sizeof (DLL_Node_t));
    MYASSERT(NewNode, CRASH_IN_CALLOC_OF_NODE, return 0)

    DLL_InitNode(NewNode);

    NewNode->Value = InsertValue;
    NewNode->Next = InsertNode->Next;
    NewNode->Prev = InsertNode;

    (InsertNode->Next)->Prev = NewNode;
    InsertNode->Next = NewNode;

    return NewNode;
}

int DLL_Erase (DLL_Node_t* EraseNode)
{
    MYASSERT(EraseNode, BAD_POINTER_PASSED_IN_FUNC, return 0)
    MYASSERT((EraseNode->Next != NULL) && (EraseNode->Prev != NULL), ERASE_HEAD_OR_TALE,return 0)

    DLL_Node_t* PrevNode = EraseNode->Prev;
    DLL_Node_t* NextNode = EraseNode->Next;

    PrevNode->Next = NextNode;
    NextNode->Prev = PrevNode;
    free(EraseNode);

    return 1;
}