#include "Definitions.h"
#include "Timer.h"
#include "Motor.h"
#include "UvLamp.h"
#include "EventManagerTask.h"

void CompleteCuringCycle();

Semaphore gSstartCuringTimerSem;
Semaphore gSancelCuringTimerSem;
Semaphore gLampOnSem;
Semaphore gLampOffSem;
Semaphore gMotorOnSem;
Semaphore gMotorOffSem;

static Timer curingCycleTimer(UVLED_DEFAULT_ON_TIME, CompleteCuringCycle);
static Motor motorController(MOTOR_EN_PIN);
static UvLamp lampController(UVLED);

/// @brief Function to call totrigger stopping the curing cycle.
void CompleteCuringCycle()
{
    gEventQ.Add(SysEvt_CuringTimerCompleted);
}


void SetupCuringHw(void* param)
{
    SelectedSettings* settings = static_cast<SelectedSettings*>(param);

    Serial.println("Curing Hardware");
    lampController.Setup();
    motorController.Setup();
    motorController.setSpeed(settings->MotorSpeed);
}

void RunCuringHw(void* param)
{
    SelectedSettings* settings = static_cast<SelectedSettings*>(param);

    if(gLampOnSem.IsSignaled())
    {
        lampController.On();
    }

    if(gLampOffSem.IsSignaled())
    {
        lampController.Off();
    }

    if(gMotorOnSem.IsSignaled())
    {
        motorController.setSpeed(settings->MotorSpeed);
        motorController.start();
    }

    if(gMotorOffSem.IsSignaled())
    {
        motorController.stop();
    }

    if(gSstartCuringTimerSem.IsSignaled())
    {
        curingCycleTimer.SetTimerLength(settings->CuringTime*ms);
        curingCycleTimer.Start();
    }

    if(gSancelCuringTimerSem.IsSignaled())
    {
        if(!curingCycleTimer.IsCompleted())
        {
            curingCycleTimer.Stop();
        }
    }
}

