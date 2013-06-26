
#ifndef NotificationHandle_H
#define NotificationHandle_H

#include <stdio.h>
#include "ECGPkh.h"
#include "TMDQueue.h"


typedef struct NotificationHandle NotificationHandle;
struct NotificationHandle
{
	UpdateFuncPtr updateAddr;
	struct NotificationHandle* itsNotificationHandle;
};

void NotificationHandle_Init(NotificationHandle* const me);
void NotificationHandle_Cleanup(NotificationHandle* const me);
struct NotificationHandle* NotificationHandle_getItsNotificationHandle(const NotificationHandle* const me);
struct NotificationHandle* NotificationHandle_Create();
void NotificationHandle_setItsNotificationHandle(const NotificationHandle* const me, struct NotificationHandle* p_NotificationHandle);
void NotificationHandle_Destroy(NotificationHandle* const me);

#endif