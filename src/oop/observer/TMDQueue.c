
#include "TMDQueue.h"


static void initRelations(TMDQueue* const me);
static void cleanUpRelations(TMDQueue* const me);

void TMDQueue_Init(TMDQueue* const me)
{
   me->head = 0;
   me->size = 0;
   initRelations(me);
}

void TMDQueue_Cleanup(TMDQueue* const me)
{
   cleanUpRelations(me);
}

int TMDQueue_getNextIndex(TMDQueue* const me, int index)
{
   return (index+1)%QUEUE_SIZE;
}

void TMDQueue_insert(TMDQueue* const me, const struct TimeMarkedData tmd)
{
   me->buffer[me->head]=tmd;
   me->head=TMDQueue_getNextIndex(me, me->head);
   if (me->size<QUEUE_SIZE)++me->size;
}

boolean TMDQueue_isEmpty(TMDQueue* const me)
{
   return (boolean)(me->size==0);
}

struct TimeMarkedData TMDQueue_remove(TMDQueue* const me, int index)
{
   TimeMarkedData tmd;
   tmd.timeInterval = -1;
   tmd.dataValue=-9999;

   if (!TMDQueue_isEmpty(me) &&
	   (index>=0) && (index < QUEUE_SIZE)
	   && index<me->size))
   {
	   tmd = me->buffer[index];
   }
   return tmd;
}

int TMDQueue_getBuffer(const TMDQueue* const me)
{
   int iter = 0;
   return iter;
}

TMDQueue* TMDQueue_Create(void)
{
   TMDQueue* me = (TMDQueue*)malloc(sizeof(TMDQueue));
   if (me != NULL)
   {
      TMDQueue_Init(me);
   }
   return me;
}

void TMDQueue_Destroy(TMDQueue* const me)
{
   if (me != NULL)
   {
      TMDQueue_Cleanup(me);
   }
   free(me);
}

static void initRelations(TMDQueue* const me)
{
   int iter = 0;
   while(iter<QUEUE_SIZE)
   {
      TimeMarketData_Init(&((me->buffer)[iter]));
      TimeMarketData__setItsTMDQueue(&((me->buffer)[iter]), me);
	  iter++;
   }
}

static void cleanUpRelations(TMDQueue* const me)
{
	int iter = 0;
	while(iter<QUEUE_SIZE)
	{
        TimeMarketData_Cleanup(&((me->buffer)[iter]));
		iter++;
	}
}