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

//=======================================================================================
//Change Depending on type of element
inline __attribute__((always_inline)) bool DLL_Compare(DLL_Elem_t val1, DLL_Elem_t val2)
{
    //occurance not depend
    //__m128i str1 = _mm_load_si128((const __m128i *) (val1.Key));
    //__m128i str2 = _mm_load_si128((const __m128i *) (val2.Key));
    //__m128i cmp  = _mm_cmpeq_epi8 (str1, str2);
    //int result   = _mm_movemask_epi8 (cmp);
    //return ((result == 0xFFFF) && (val1.Value == val2.Value));
    return !((strcmp(val1.Key,val2.Key)) || (val1.Value != val2.Value)); 
}
//=======================================================================================

const DLL_Elem_t POISON_ELEMENT = {NULL, 0, 0};

#endif