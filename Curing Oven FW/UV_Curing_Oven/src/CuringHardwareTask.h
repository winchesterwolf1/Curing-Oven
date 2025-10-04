#include "Definitions.h"

#ifndef CURINGHARDWARETASK_H
#define CURINGHARDWARETASK_H

extern Semaphore gSstartCuringTimerSem;
extern Semaphore gSancelCuringTimerSem;
extern Semaphore gLampOnSem;
extern Semaphore gLampOffSem;
extern Semaphore gMotorOnSem;
extern Semaphore gMotorOffSem;

void SetupCuringHw(void* param);
void RunCuringHw(void* param);

#endif // CURINGHARDWARETASK_H