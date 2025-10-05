#include "Task_CuringHardware.h"
#include "Timer.h"
#include "Motor.h"
#include "UvLamp.h"

/// @brief Task for running the hardware and timers used to run a curing cycle
class CuringHardwareTask : public Task
{
    public:

        /// @brief setup default values and pin mapping for this task.
        CuringHardwareTask() : 
            Task(Task::Priority::High),
            _curingCycleTimer(UVLED_DEFAULT_ON_TIME, CureCompleteTimerCallbackWrapper, this),
            _motorController(MOTOR_EN_PIN),
            _lampController(UVLED)

        {

        }

        /// @brief Setup registers and initial states for all outputs
        /// @param param application environment information object
        void Setup(void* param)
        {
            // Cast object to be usable
            SelectedSettings* settings = static_cast<SelectedSettings*>(param);

            Serial.println("Curing Hardware");
            // Run Setup functions
            _lampController.Setup();
            _motorController.Setup();
            _motorController.setSpeed(settings->MotorSpeed);
        }

        /// @brief Check all semaphores and react to commands from the event manager
        /// @param param application environment information object
        void RunTask(void* param)
        {
            // Cast object to be usable
            SelectedSettings* settings = static_cast<SelectedSettings*>(param);

            // Check hardware control semaphores and react acordingly
            if(_pRtos->Semaphores[Sem_LampOn]->IsSignaled())
            {
                _lampController.On();
            }

            if(_pRtos->Semaphores[Sem_LampOff]->IsSignaled())
            {
                _lampController.Off();
            }

            if(_pRtos->Semaphores[Sem_MotorOn]->IsSignaled())
            {
                _motorController.setSpeed(settings->MotorSpeed);
                _motorController.start();
            }

            if(_pRtos->Semaphores[Sem_MotorOff]->IsSignaled())
            {
                _motorController.stop();
            }

            if(_pRtos->Semaphores[Sem_StartCuringTimer]->IsSignaled())
            {
                _curingCycleTimer.SetTimerLength(settings->CuringTime*ms);
                _curingCycleTimer.Start();
            }

            if(_pRtos->Semaphores[Sem_CancelCuringTimer]->IsSignaled())
            {
                if(!_curingCycleTimer.IsCompleted())
                {
                    _curingCycleTimer.Stop();
                }
            }
        }

    private:

        /// @brief Timer for controlling the end of a curing cycle
        Timer _curingCycleTimer;
        /// @brief Motor controller for turning the cure plate
        Motor _motorController;
        /// @brief Lamp Controller to switching the UV bulb on and off
        UvLamp _lampController;

        /// @brief Function to call to trigger stopping the curing cycle.
        void CompleteCuringCycle()
        {
            SysEvt_t evt = SysEvt_CuringTimerCompleted;
            _pRtos->Queues[Queues_SysEvt]->Add(&evt);
        }

        /// @brief Static wrapper method to call when the timer completes
        /// @param obj this task object
        static void CureCompleteTimerCallbackWrapper(void* obj)
        {
            CuringHardwareTask* task = static_cast<CuringHardwareTask*>(obj);
            task->CompleteCuringCycle();
        }
};

/// @brief Instance of the Curing Hardware Task
CuringHardwareTask curingHardwareTaskObj;
/// @brief Base Task pointer of Curing Hardware Task to be used globaly for pRtos initialisation
Task* gCuringHardwareTaskPtr = &curingHardwareTaskObj;
