#ifndef HTTYPES_H
#define HTTYPES_H

typedef const char* HT_Key_t;
const HT_Key_t HT_KEY_POISON_ELEMENT = NULL;

typedef size_t HT_Value_t;
const HT_Value_t HT_VALUE_POISON_ELEMENT = 0;

typedef struct DLL_Elem {
    size_t      Hash;
    HT_Key_t    Key;
    HT_Value_t  Value;      //Value in my lab == occurance
    size_t      Occurance;
}  DLL_Elem_t;

bool DLL_Compare(DLL_Elem_t val1, DLL_Elem_t val2);

const DLL_Elem_t POISON_ELEMENT = {0, NULL, 0};

#endif