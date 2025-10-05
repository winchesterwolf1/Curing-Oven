#include "Task_CuringHardware.h"
#include "Timer.h"
#include "Motor.h"
#include "UvLamp.h"

class CuringHardwareTask : public Task
{
    public:

    CuringHardwareTask() : 
        Task(1),
        curingCycleTimer(UVLED_DEFAULT_ON_TIME, CureCompleteTimerCallbackWrapper, this),
        motorController(MOTOR_EN_PIN),
        lampController(UVLED)

    {

    }

    Timer curingCycleTimer;
    Motor motorController;
    UvLamp lampController;

    /// @brief Function to call to trigger stopping the curing cycle.
    void CompleteCuringCycle()
    {
        SysEvt_t evt = SysEvt_CuringTimerCompleted;
        _pRtos->Queues[Queues_SysEvt]->Add(&evt);
    }

    /// @brief Static wrapper method to call when the timer completes
    /// @param obj 
    static void CureCompleteTimerCallbackWrapper(void* obj)
    {
        CuringHardwareTask* task = static_cast<CuringHardwareTask*>(obj);
        task->CompleteCuringCycle();
    }

    void Setup(void* param)
    {
        SelectedSettings* settings = static_cast<SelectedSettings*>(param);

        Serial.println("Curing Hardware");
        lampController.Setup();
        motorController.Setup();
        motorController.setSpeed(settings->MotorSpeed);
    }

    void RunTask(void* param)
    {
        SelectedSettings* settings = static_cast<SelectedSettings*>(param);

        if(_pRtos->Semaphores[Sem_LampOn]->IsSignaled())
        {
            lampController.On();
        }

        if(_pRtos->Semaphores[Sem_LampOff]->IsSignaled())
        {
            lampController.Off();
        }

        if(_pRtos->Semaphores[Sem_MotorOn]->IsSignaled())
        {
            motorController.setSpeed(settings->MotorSpeed);
            motorController.start();
        }

        if(_pRtos->Semaphores[Sem_MotorOff]->IsSignaled())
        {
            motorController.stop();
        }

        if(_pRtos->Semaphores[Sem_StartCuringTimer]->IsSignaled())
        {
            curingCycleTimer.SetTimerLength(settings->CuringTime*ms);
            curingCycleTimer.Start();
        }

        if(_pRtos->Semaphores[Sem_CancelCuringTimer]->IsSignaled())
        {
            if(!curingCycleTimer.IsCompleted())
            {
                curingCycleTimer.Stop();
            }
        }
    }
};

CuringHardwareTask curingHardwareTaskObj;
Task* gCuringHardwareTaskPtr = &curingHardwareTaskObj;
