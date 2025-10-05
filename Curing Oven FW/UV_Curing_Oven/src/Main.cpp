#include "Definitions.h"
#include "TimeServer.h"
#include "Task_CuringHardware.h"
#include "Task_EventManager.h"
#include "Task_Ui.h"

// Create System environment info object
static SelectedSettings settings;
// Create Pseudo RTOS object
static PseudoRtos pRtos(&settings);

/// @brief Initialise application
void setup() 
{
    Serial.begin(9600);
    
    Serial.println("Initialising Time Server");
    TimeServerInit();
        
    // Initialise pRtos Tasks
    pRtos.RegisterTask(gEventManagerTaskPtr);
    pRtos.RegisterTask(gUiTaskPtr);
    pRtos.RegisterTask(gCuringHardwareTaskPtr);

    // Initialise pRtos Queues
    static SysEvtQueue evtQueue;
    pRtos.RegisterQueue(&evtQueue, Queues_SysEvt);

    // Initialise pRtos Semaphores
    static Semaphore cancelCuringTimer;
    pRtos.RegisterSemaphore(&cancelCuringTimer, Sem_CancelCuringTimer);
    static Semaphore lampOff;
    pRtos.RegisterSemaphore(&lampOff, Sem_LampOff);
    static Semaphore lampOn;
    pRtos.RegisterSemaphore(&lampOn, Sem_LampOn);
    static Semaphore motorOff;
    pRtos.RegisterSemaphore(&motorOff, Sem_MotorOff);
    static Semaphore motorOn;
    pRtos.RegisterSemaphore(&motorOn, Sem_MotorOn);
    static Semaphore startCuringTimer;
    pRtos.RegisterSemaphore(&startCuringTimer, Sem_StartCuringTimer);
    static Semaphore uiNotifyCompleted;
    pRtos.RegisterSemaphore(&uiNotifyCompleted, Sem_UiNotifyCompleted);

    // Initialise the pRtos
    pRtos.Init();
}

/// @brief Main loop, simply run the pRtos
void loop() 
{
    pRtos.RunThrough();
}
