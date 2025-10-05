#include "Definitions.h"
#include "TimeServer.h"
#include "Task_CuringHardware.h"
#include "Task_EventManager.h"
#include "Task_Ui.h"

static SelectedSettings settings;
static PseudoRtos pRtos(&settings);

void setup() 
{
    Serial.begin(9600);
    
    Serial.println("Initialising Time Server");
    TimeServerInit();
        

    pRtos.RegisterTask(gEventManagerTaskPtr);
    pRtos.RegisterTask(gUiTaskPtr);
    pRtos.RegisterTask(gCuringHardwareTaskPtr);

    static SysEvtQueue evtQueue;
    pRtos.RegisterQueue(&evtQueue, Queues_SysEvt);

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

    pRtos.Init();
}

void loop() 
{
    pRtos.RunThrough();
}
