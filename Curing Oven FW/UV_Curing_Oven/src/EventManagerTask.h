#include "Definitions.h"

#ifndef EVENT_MANAGER_TASK_H
#define EVENT_MANAGER_TASK_H

extern SysEvtQueue gEventQ;

void SetupEventManager(void* param);
void RunEventManager(void* param);

#endif // EVENT_MANAGER_TASK_H