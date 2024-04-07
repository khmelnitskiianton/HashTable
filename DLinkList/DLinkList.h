#ifndef DLINKLIST_H
#define DLINKLIST_H
  
typedef char* DLL_Elem_t;
#define SPECIFIER "%s"
const DLL_Elem_t POISON_ELEMENT = NULL;

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
DLL_Node_t* DLL_Find        (DLL_Elem_t Value, DLL_LinkList_t* myLinkList);

#endif
