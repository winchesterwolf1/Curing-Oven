#include "Definitions.h"
#include "EncoderKnob.h"
#include "Ui.h"
#include "UiImplementation.h"
#include "PWM.h"
#include "TimeServer.h"

int counter = 0;
int menu_state = 0;

void LedStop();
void LedStart();
void MotorStart();
void MotorStop();

void(*systemFunctions[4])();


void SetScreenRefreshFlag();

static SelectedSettings _settings;
static Timer _uvLedOnTimer(UVLED_DEFAULT_ON_TIME, LedStop);

Ui* UiObj;

void setup() 
{
    Serial.begin(9600);
    
    Serial.println("Initialising Time Server");
    TimeServerInit();
    
    Serial.println("Initialising LED Output");
    pinMode(UVLED, OUTPUT);
    digitalWrite(UVLED, LOW);
    
    Serial.println("Initialising PWM");
    init_PWM();
    
    Serial.println("Initialising UI");
    systemFunctions[SysFuncLedStart] = LedStart;
    systemFunctions[SysFuncLedStop] = LedStop;
    systemFunctions[SysFuncMotorStart] = MotorStart;
    systemFunctions[SysFuncMotorStop] = MotorStop;

    UiObj = CreateUi(&_settings, systemFunctions);  
    
    Serial.println("Starting Screen Refresh Timer");
}

void LedStop()
{
    Serial.println("Stop UV LED");
    digitalWrite(UVLED, LOW);
    if(!_uvLedOnTimer.IsCompleted())
    {
        _uvLedOnTimer.Stop();
    }
}

void LedStart()
{
    Serial.println("Start UV LED");    
    digitalWrite(UVLED, HIGH);
    _uvLedOnTimer.SetTimerLength(_settings.on_time*ms);
    _uvLedOnTimer.Start();
}

void MotorStart()
{}
void MotorStop()
{}

void loop() 
{
    UiUpdate(UiObj);
    //update_LED();
}
