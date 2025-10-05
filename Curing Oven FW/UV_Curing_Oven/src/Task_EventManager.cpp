#include "Task_EventManager.h"

/// @brief Event manager task to read new events and communicate to other tasks to trigger reactions
class EventManagerTask : public Task
{
    public:
        /// @brief Constructor to save default parameters. 
        /// This task should be run first, so it is getting the highest priority
        EventManagerTask() :
            Task(Task::Priority::Highest)
        {

        }

        /// @brief Not needed for this task
        void Setup(void* param)
        {
            // Empty Not Implemented
        }

        /// @brief Read events from the queue and set semaphores accordingly
        /// @param param not used
        void RunTask(void* param)
        {
            // Read every event that has queued up since last run and service them
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

/// @brief Instance of the Event Manager Task
EventManagerTask eventManagerTaskObj;
/// @brief Base Task pointer of Event Manager Task to be used globaly for pRtos initialisation
Task* gEventManagerTaskPtr = &eventManagerTaskObj;
