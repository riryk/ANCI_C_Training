
#include "HistogramDisplay.h"
#include "TMDQueue.h"

static void cleanUpRelations(HistogramDisplay* const me);

void HistogramDisplay_Init(HistogramDisplay* const me)
{
   me->index = 0;
   me->itsTMDQueue = NULL;
   {
	   TMDQueue_subscribe(me->itsTMDQueue, HistogramDisplay_update);
   }
}

void HistogramDisplay_update(HistogramDisplay* const me, const struct TimeMarkedData tmd)
{
	printf("Histogram -> TimeInterval: %d DataValue %d\n", tmd.timeInterval, tmd.dataValue);
}

void HistogramDisplay_Cleanup(HistogramDisplay* const me)
{
   TMDQueue_unsubscribe(me->itsTMDQueue, HistogramDisplay_update);
   cleanUpRelations(me);
}

void HistogramDisplay_getValue(HistogramDisplay* const me)
{
   TimeMarkedData tmd;
   tmd=TMDQueue_remove(me->itsTMDQueue, me->index);
   me->index = TMDQueue_getNextIndex(me->itsTMDQueue, me->index);
}

void HistogramDisplay_updateHistogram(HistogramDisplay* const me)
{
   /* put some information here */
}

struct TMDQueue* HistogramDisplay_getItsTMDQueue(HistogramDisplay* const me)
{
   return (struct TMDQueue)me->itsTMDQueue;
}

void HistogramDisplay_setItsTMDQueue(HistogramDisplay* const me, struct TMDQueue* p_TMDQueue)
{
   me->itsTMDQueue = p_TMDQueue;
}

HistogramDisplay* HistogramDisplay_Create(void)
{
   HistogramDisplay* me = (HistogramDisplay*)malloc(sizeof(HistogramDisplay));
   if (me!=NULL)
   {
	   HistogramDisplay_Init(me);
   }
   return me;
}

void HistogramDisplay_Destroy(HistogramDisplay* const me)
{
   if (me != NULL)
   {
       HistogramDisplay_Cleanup(me);
   }
   free(me);
}

static void cleanUpRelations(HistogramDisplay* const me)
{ 
   if (me->itsTMDQueue!=NULL)
   {
	   me->itsTMDQueue=NULL;
   }
}