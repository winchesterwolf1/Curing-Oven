#include "Definitions.h"
#include "TimeServer.h"
#include "CuringHardwareTask.h"
#include "EventManagerTask.h"
#include "UiTask.h"

static SelectedSettings settings;
static PseudoRtos pRtos;

static Task eventManagerTask(SetupEventManager, RunEventManager, &settings, 0);
static Task uiTask(SetupUi, RunUi, &settings, 1);
static Task curingHwTask(SetupCuringHw, RunCuringHw, &settings, 1);

void setup() 
{
    Serial.begin(9600);
    
    Serial.println("Initialising Time Server");
    TimeServerInit();
        

    pRtos.RegisterTask(&eventManagerTask);
    pRtos.RegisterTask(&uiTask);
    pRtos.RegisterTask(&curingHwTask);

    pRtos.Init();
}

void loop() 
{
    pRtos.RunThrough();
}
