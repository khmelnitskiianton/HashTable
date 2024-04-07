#ifndef CREATOR_H
#define CREATOR_H

typedef struct Line {
    size_t Length;
    char*  StartLine;
} Line_t;

typedef struct Information 
{   
    FILE*   file_text;
    FILE*   file_write;

    size_t  size_text;
    size_t  n_strings;

    char*   text_buffer;
    Line_t* string_buffer;
} Information_t;

EnumOfErrors    InfoCtor    (Information_t* InfoData, const char* file_read, const char* file_write);
EnumOfErrors    InfoDtor    (Information_t* InfoData);
FILE*           OpenFile    (const char* file_open, const char* option);
void            CloseFile   (FILE* file_text);
size_t          FileSize    (FILE *file_text);

#endif