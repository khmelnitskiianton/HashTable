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

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

//=======================================================================================
//Change Depending on type of element
inline __attribute__((always_inline)) bool DLL_Compare(DLL_Elem_t val1, DLL_Elem_t val2)
{
    //occurance not depend
    return !((strcmp(val1.Key,val2.Key)) || (val1.Value  != val2.Value)); 
    //return !((memcmp(val1.Key,val2.Key, MIN(strlen(val1.Key), strlen(val2.Key)))) || (val1.Value  != val2.Value));
}
//=======================================================================================

const DLL_Elem_t POISON_ELEMENT = {0, NULL, 0};

#endif