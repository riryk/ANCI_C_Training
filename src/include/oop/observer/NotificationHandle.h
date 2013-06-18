
#ifndef NotificationHandle_H
#define NotificationHandle_H

#include <stdio.h>
#include "ECGPkg.h"

typedef struct NotificationHandle NotificationHandle;
struct NotificationHandle
{
	UpdateFuncPtr updateAddr;
	struct NotificationHandle* itsNotificationHandle;
};

void NotificationHandle_Init(NotificationHandle* const me);
void NotificationHandle_Cleanup(NotificationHandle* const me);


#endif