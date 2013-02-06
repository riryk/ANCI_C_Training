#include "test.h"
#include <stdio.h>
<<<<<<< HEAD

int main(int argc, char *argv[])
{
	int c;

	while ((c = getchar()) != EOF)
       putchar(c);

=======
#include <windows.h>

#define _IOREAD_COPY            0x0001
#define _IOWRT_COPY             0x0002
#define _IOYOURBUF_COPY         0x0004


#define BUF_SIZE           5*1024
#define _IOB_ENTRIES_COPY  20

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

void* malloc_copy(unsigned int size)
{
	return NULL;
}

int main(int argc, char *argv[])
{
	void* A = malloc(122);

	char charsBuf[BUF_SIZE]="adsadasdasdasd";
    LPDWORD bytesWritten;
    ULONG dosretval;

	
	FILE_COPY stdout1 = _iob_copy[0];

 	/*if (!WriteFile(
		   NULL,
           charsBuf,
           BUF_SIZE,
           bytesWritten,
		   NULL))
	{
        dosretval = GetLastError();
	}*/

         
    printf("hello"); 
>>>>>>> origin/master
    test_method("");
}