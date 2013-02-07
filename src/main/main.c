#include "inout.h"

#define _IOREAD_COPY            0x0001
#define _IOWRT_COPY             0x0002
#define _IOYOURBUF_COPY         0x0004


#define HEAP_SIZE          0x01000
#define BUF_SIZE           5*1024
#define _IOB_ENTRIES_COPY  20
#define EOF_COPY     (-1)

struct _io_file_copy
{
        char *_ptr_copy;
        int   _cnt_copy;
        char *_base_copy;
        int   _flag_copy;
        int   _file_copy;
        int   _charbuf_copy;
        int   _bufsiz_copy;
        char *_tmpfname_copy;
};

typedef struct _io_file_copy FILE_COPY;

char _bufin_copy[BUF_SIZE];

/*
 * FILE_COPY descriptors; preset for stdin/out/err (note that the __tmpnum field
 * is not initialized)
 */
FILE_COPY _iob_copy[_IOB_ENTRIES] = 
{
        /* _ptr, _cnt, _base,  _flag, _file, _charbuf, _bufsiz */
        /* stdin (_iob[0]) */
        { _bufin_copy, 0, _bufin_copy, _IOREAD_COPY | _IOYOURBUF_COPY, 0, 0, BUF_SIZE },
        /* stdout (_iob[1]) */
        { NULL, 0, NULL, _IOWRT_COPY, 1, 0, 0 },
        /* stderr (_iob[3]) */
        { NULL, 0, NULL, _IOWRT_COPY, 2, 0, 0 },
};

#define stdin_copy  (_iob_copy[0])
#define stdout_copy (_iob_copy[1])
#define stderr_copy (_iob_copy[2])

/* Buffer pointers for stdout and stderr */
void *_stdbuf_copy[2] = { NULL, NULL};

HANDLE myHeap;

void CreateMyHeap()
{
	myHeap = HeapCreate(0, HEAP_SIZE, 0);
}

void DeleteMyHEap()
{
	if (myHeap != NULL)
	{
		HeapDestroy(myHeap);
	}
}

void* malloc_copy(unsigned int size)
{
	if (myHeap != NULL)
	{
		return (void*)HeapAlloc(myHeap, 0, size);
	}

	return NULL;
}

int _fputc_copy(char _c, FILE_COPY _stream)    
{
	if (--(_stream)._cnt_copy >= 0)
	{
		return 0xff & (*(_stream)._ptr_copy++ = (char)(_c));
	}
	
	return NULL;

	//return _flsbuf((_c),(_stream));
}

int main(int argc, char *argv[])
{
	void* A = malloc(122);

	char charsBuf[BUF_SIZE]="adsadasdasdasd";
    LPDWORD bytesWritten;
    ULONG dosretval;
	int c;
	int isInvalid;
	FILE_COPY stdout1;

	HANDLE consoleHandler = GetStdHandle(STD_INPUT_HANDLE); 
	isInvalid = consoleHandler == INVALID_HANDLE_VALUE;
	
	stdout1 = _iob_copy[0];

	stdout1._ptr_copy = stdout1._base_copy = (char*)malloc_copy(BUF_SIZE);
	stdout1._cnt_copy = stdout1._bufsiz_copy = BUF_SIZE;

 	/*if (!WriteFile(
		   consoleHandler,
           charsBuf,
           BUF_SIZE,
           bytesWritten,
		   NULL))
	{
        dosretval = GetLastError();
	}

	CloseHandle(consoleHandler);*/
         
    printf("hello"); 

    test_method("");

	while ((c = getchar()) != EOF)
       putchar(c);
}