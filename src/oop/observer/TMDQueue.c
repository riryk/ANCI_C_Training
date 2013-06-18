
#include "TMDQueue.h"
#include "NotificationHandle.h"

static void initRelations(TMDQueue* const me);
static void cleanUpRelations(TMDQueue* const me);

void TMDQueue_Init(TMDQueue* const me)
{
   me->head = 0;
   me->nSubscribers = 0;
   me->size = 0;
   me->itsNotificationHandle = NULL;
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

void TMDQueue_notify(TMDQueue* const me, const struct TimeMarkedData tmd)
{
   NotificationHandle* pNH;
   pNH=me->itsNotificationHandle;
   while(pNH)
   {
      pNH->updateAddr(NULL,tmd);
      pNH=pNH->itsNotificationHandle;
   }
}

void TMDQueue_insert(TMDQueue* const me, const struct TimeMarkedData tmd)
{
   me->buffer[me->head]=tmd;
   me->head=TMDQueue_getNextIndex(me, me->head);
   if (me->size<QUEUE_SIZE)++me->size;
   TMDQueue_notify(me, tmd);
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

void TMDQueue_subscribe(TMDQueue* const me, const UpdateFuncPtr updateFuncAddr)
{
   struct NotificationHandle* pNH;
   pNH=me->itsNotificationHandle;

   if (!pNH)
   {
       me->itsNotificationHandle=NotificationHandle_Create();
	   pNH=me->itsNotificationHandle;
   }
   else
   {
       while (pNH->itsNotificationHandle != NULL)   
          pNH = pNH->itsNotificationHandle;
	   pNH->itsNotificationHandle=NotificationHandle_Create();
       pNH = pNH->itsNotificationHandle;
   }
   pNH->updateAddr=updateFuncAddr;
   ++me->nSubscribers;
}

int TMDQueue_unsubscribe(TMDQueue* const me, const UpdateFuncPtr updateFuncAddr)
{
   struct NotificationHandle *pNH, *pBack;
   pNH=pBack=me->itsNotificationHandle;
   if (pNH==NULL)
   {
	   return 0;
   }
   else
   {
	   if (pNH->updateAddr==updateFuncAddr)
	   {
		   me->itsNotificationHandle = pNH->itsNotificationHandle;
		   free(pNH);
		   --me->nSubscribers;
		   return 1;
	   }
	   else
	   {
           while (pNH!=NULL)
		   {
               if (pNH->updateAddr==updateFuncAddr)
			   {
                   pBack->itsNotificationHandle = pNH->itsNotificationHandle;
				   free(pNH);
				   --me->nSubscribers;
				   return 1;
			   }
			   pBack = pNH;
               pNH = pNH->itsNotificationHandle;
		   }
	   }
   }
}

int TMDQueue_getBuffer(const TMDQueue* const me)
{
   int iter = 0;
   return iter;
}

struct NotificationHandle* TMDQueue_getItsNotificationHandle(TMDQueue* const me)
{
	return (struct NotificationHandle*)me->itsNotificationHandle;
}

void TMDQueue_setItsNotificationHandle(TMDQueue* const me, struct NotificationHandle* p_NotificationHandle)
{
    me->itsNotificationHandle=p_NotificationHandle;
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