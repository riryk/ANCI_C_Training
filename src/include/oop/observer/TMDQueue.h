
#ifndef TMDQueue_H
#define TMDQueue_H

#include <stdio.h>
#include "ECGPkh.h"
#include "TimeMarkedData.h"

struct NotificationHandle;

typedef struct TMDQueue TMDQueue;

struct TMDQueue
{
	int head;
	int nSubscribers;
	int size;
	struct TimeMarkedData buffer[QUEUE_SIZE];
    struct NotificationHandle* itsNotificationHandle;
};

void TMDQueue_Init(TMDQueue* const me);
void TMDQueue_Cleanup(TMDQueue* const me);

int TMDQueue_getNextIndex(TMDQueue* const me, int index);
void TMDQueue_insert(TMDQueue* const me, const struct TimeMarkedData tmd);
boolean TMDQueue_isEmpty(TMDQueue* const me);
struct TimeMarkedData TMDQueue_remove(TMDQueue* const me, int index);

void TMDQueue_subscribe(TMDQueue* const me, const UpdateFuncPtr updateFuncAddr);
int TMDQueue_unsubscribe(TMDQueue* const me, const UpdateFuncPtr updateFuncAddr);
struct NotificationHandle* TMDQueue_getItsNotificationHandle(TMDQueue* const me);
void TMDQueue_setItsNotificationHandle(TMDQueue* const me, struct NotificationHandle* p_NotificationHandle);

int TMDQueue_getBuffer(const TMDQueue* const me);
TMDQueue* TMDQueue_Create(void);
void TMDQueue_Destroy(TMDQueue* const me);

struct NotificationHandle NotificationHandle_Create();

#endif