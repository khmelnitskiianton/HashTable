#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "myassert.h"
#include "creator.h"

static const size_t ALIGNING = 16;

EnumOfErrors InfoCtor (Information_t* InfoData, const char* file_read, const char* file_write)
{
    MYASSERT(file_read, ERR_WHAT_FILE_OF_DATA, return ERR_WHAT_FILE_OF_DATA)
    MYASSERT(file_write, ERR_WHAT_FILE_OF_DATA, return ERR_WHAT_FILE_OF_DATA)

    InfoData->file_text     = OpenFile (file_read, "rb");
    InfoData->file_write    = OpenFile (file_write, "wb");

    USER_ERROR(InfoData->file_text,  ERR_NO_FILE_TO_OPEN, "", exit(0))
    USER_ERROR(InfoData->file_write, ERR_NO_FILE_TO_OPEN, "", exit(0))

    InfoData->size_text = FileSize (InfoData->file_text);

    InfoData->text_buffer = (char*) calloc (InfoData->size_text + 16, sizeof (char));
    MYASSERT(InfoData->text_buffer, ERR_BAD_CALLOC, return ERR_BAD_CALLOC)
    size_t result_size = fread (InfoData->text_buffer, 1, InfoData->size_text, InfoData->file_text);

    MYASSERT(result_size == (InfoData->size_text), ERR_BAD_FREAD, return ERR_BAD_FREAD);

    USER_ERROR(*(InfoData->text_buffer) != '\0', ERR_EMPTY_FILE, "", return ERR_EMPTY_FILE)
    
	*((InfoData->text_buffer) + (InfoData->size_text)) = '\0';

    //string buffer
	InfoData->n_strings = 0;
	for (size_t i = 0; i < (InfoData->size_text); i++)
    {
        if ((*((InfoData->text_buffer) + i) == '\0') || (*((InfoData->text_buffer) + i) == '\n'))
        {
            InfoData->n_strings += 1;
        }
    }
	if ((InfoData->string_buffer  = (Line_t*) calloc ((InfoData->n_strings)+1, sizeof (Line_t))) == nullptr)
	{
		assert(0);
	}
	char *symbol = InfoData->text_buffer;
	size_t p = 0;
	size_t amount = 0;
	size_t length = 0;
	(*(InfoData->string_buffer)).StartLine = InfoData->text_buffer;
	while (*(symbol + p) != '\0')
	{
		length++;
		if ((*(symbol + p) == '\n') && (*(symbol + p + 1) != '\0'))
		{
			(*(InfoData->string_buffer + amount)).Length = length-2;
			amount++;
			length = 0;
			(*(InfoData->string_buffer + amount)).StartLine = symbol + p + 1;
		}
		p++;
	}
	(*(InfoData->string_buffer + amount)).Length = length;

    char* ptr_buffer = InfoData->text_buffer;
    
    size_t index = 0;
    while(ptr_buffer[index] != '\0')
    {
        if (ptr_buffer[index] == '\n') ptr_buffer[index] = '\0';
        index++;
    }
    //==============================
    //aligning
    char* aligned_buffer = (char*) aligned_alloc (ALIGNING, sizeof (char) * ALIGNING * ((InfoData->n_strings)+1));
    memset(aligned_buffer, 0, sizeof (char) * ALIGNING * ((InfoData->n_strings)+1));
    for (size_t i = 0; i < (InfoData->n_strings);i++)
    {
        memcpy((aligned_buffer + ALIGNING*i), (InfoData->string_buffer+i)->StartLine, (InfoData->string_buffer+i)->Length);
        (InfoData->string_buffer+i)->StartLine = (aligned_buffer + ALIGNING*i);
    }
    free(InfoData->text_buffer);
    InfoData->text_buffer = aligned_buffer;
    //==============================
    return ERR_OK;   
}

EnumOfErrors InfoDtor (Information_t* InfoData)
{
    CloseFile (InfoData->file_text);
    CloseFile (InfoData->file_write);
    free(InfoData->text_buffer);
    free(InfoData->string_buffer);
    return ERR_OK;
}

FILE* OpenFile (const char* file_open, const char* option)
{
    FILE *FileOpen = fopen (file_open, option);

    MYASSERT(FileOpen, OPEN_FILE, return NULL);

    return FileOpen;
}

void CloseFile (FILE* file_text)
{
	MYASSERT(file_text, BAD_POINTER_PASSED_IN_FUNC, assert(0));
    int result = fclose(file_text);
	MYASSERT(!result, CLOSE_FILE, assert(0));
}

size_t FileSize (FILE *file_text)
{
    MYASSERT(file_text, ERR_BAD_POINTER_FILE, return 0)
	struct stat st;
    int fd = fileno (file_text); 
    fstat (fd, &st);
    size_t size_text = (size_t) st.st_size;
	return size_text;
}