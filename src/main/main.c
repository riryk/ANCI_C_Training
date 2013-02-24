#include "memory.h"

int main(int argc, char *argv[])
{
	int* Aint;
    int* Aint1;
	int* Aint2;
	int* Aint3;
    int* Aint4;
	int* Aint5;
	int* Aint6;
    int* Aint7;


	Aint = (int*)LocMalloc(5*sizeof(int));
    Aint1 = (int*)LocMalloc(6*sizeof(int));
    Aint2 = (int*)LocMalloc(7*sizeof(int));
    Aint3 = (int*)LocMalloc(8*sizeof(int));
    Aint4 = (int*)LocMalloc(9*sizeof(int));
    Aint5 = (int*)LocMalloc(10*sizeof(int));
    Aint6 = (int*)LocMalloc(11*sizeof(int));

	LocFree(Aint);
    LocFree(Aint1);
    LocFree(Aint2);
    LocFree(Aint3);
    LocFree(Aint4);
    LocFree(Aint5);
    LocFree(Aint6);

    Aint7 = (int*)LocMalloc(7*sizeof(int));
    LocFree(Aint7);

	//Header* mem = NewMemoryChunkFromOperSyst(1);
    return 0;
}

