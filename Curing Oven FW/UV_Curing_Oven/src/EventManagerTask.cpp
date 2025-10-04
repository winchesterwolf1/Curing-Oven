#include "EventManagerTask.h"
#include "CuringHardwareTask.h"
#include "UiTask.h"

SysEvtQueue gEventQ;

void SetupEventManager(void* param)
{

}

void RunEventManager(void* param)
{
    while(!gEventQ.IsEmpty())
    {
        SysEvt_t evt;
        gEventQ.Read(&evt);
        switch (evt)
        {
            case SysEvt_StartCuringPressed:
                Serial.println("Curing Cycle Started");    
                gLampOnSem.Signal();
                gMotorOnSem.Signal();
                gSstartCuringTimerSem.Signal();
                break;

            case SysEvt_CancelCuringPressed:
                Serial.println("Curing Canceled");
                gLampOffSem.Signal();
                gMotorOffSem.Signal();
                gSancelCuringTimerSem.Signal();
                break;

            case SysEvt_CuringTimerCompleted:
                Serial.println("Curing Finished");
                gLampOffSem.Signal();
                gMotorOffSem.Signal();
                gUiCompletedSem.Signal();
                break;
        }
    }
}