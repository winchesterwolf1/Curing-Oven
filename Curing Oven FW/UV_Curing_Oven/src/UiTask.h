#include "Definitions.h"

#ifndef UI_TASK_H
#define UI_TASK_H

extern Semaphore gUiCompletedSem;

void SetupUi(void* param);
void RunUi(void* param);

#endif // UI_TASK_H