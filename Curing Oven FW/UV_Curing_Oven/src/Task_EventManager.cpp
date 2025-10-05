#include "Task_EventManager.h"

class EventManagerTask : public Task
{
    public:

    EventManagerTask() :
        Task(0)
    {

    }

    void Setup(void* param)
    {

    }

    void RunTask(void* param)
    {
        while(!_pRtos->Queues[Queues_SysEvt]->IsEmpty())
        {
            SysEvt_t evt;
            _pRtos->Queues[Queues_SysEvt]->Read(&evt);
            switch (evt)
            {
                case SysEvt_StartCuringPressed:
                    Serial.println("Curing Cycle Started");    
                    _pRtos->Semaphores[Sem_LampOn]->Signal();
                    _pRtos->Semaphores[Sem_MotorOn]->Signal();
                    _pRtos->Semaphores[Sem_StartCuringTimer]->Signal();
                    break;

                case SysEvt_CancelCuringPressed:
                    Serial.println("Curing Canceled");
                    _pRtos->Semaphores[Sem_LampOff]->Signal();
                    _pRtos->Semaphores[Sem_MotorOff]->Signal();
                    _pRtos->Semaphores[Sem_CancelCuringTimer]->Signal();
                    break;

                case SysEvt_CuringTimerCompleted:
                    Serial.println("Curing Finished");
                    _pRtos->Semaphores[Sem_LampOff]->Signal();
                    _pRtos->Semaphores[Sem_MotorOff]->Signal();
                    _pRtos->Semaphores[Sem_UiNotifyCompleted]->Signal();
                    break;
                default:
                    Serial.println("Unknown Event");
            }
        }
    }
};

EventManagerTask eventManagerTaskObj;
Task* gEventManagerTaskPtr = &eventManagerTaskObj;
