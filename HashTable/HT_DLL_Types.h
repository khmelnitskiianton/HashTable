#ifndef HTTYPES_H
#define HTTYPES_H

typedef const char* HT_Key_t;
const HT_Key_t HT_KEY_POISON_ELEMENT = NULL;

typedef size_t HT_Value_t;
const HT_Value_t HT_VALUE_POISON_ELEMENT = 0;

typedef struct DLL_Elem {
    HT_Key_t    Key;
    size_t      Hash;
    HT_Value_t  Value;      //Value in my lab == length of string
    size_t      Occurance;
}  DLL_Elem_t;

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

const DLL_Elem_t POISON_ELEMENT = {NULL, 0, 0};

#endif