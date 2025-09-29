#include "Definitions.h"
#include "EncoderKnob.h"
#include "Ui.h"
#include "UiImplementation.h"
#include "TimeServer.h"
#include "Motor.h"
#include "Sound.h"

int counter = 0;
int menu_state = 0;
int motor_Speed = 0;

void LedStop();
void LedStart();
void MotorStart();
void MotorStop();
void SpeakerFinishedBeep();

void(*systemFunctions[4])();


void SetScreenRefreshFlag();

static SelectedSettings _settings;
static Timer _uvLedOnTimer(UVLED_DEFAULT_ON_TIME, LedStop);
static Motor _motorController(MOTOR_EN_PIN);
static Sound _soundController(SPEAKER_PIN);

Ui* UiObj;

void setup() 
{
    Serial.begin(9600);
    
    Serial.println("Initialising Time Server");
    TimeServerInit();
    
    Serial.println("Initialising LED Output");
    pinMode(UVLED, OUTPUT);
    digitalWrite(UVLED, LOW);
    
    Serial.println("Initialising Motor");
    _motorController.setSpeed(_settings.motor_speed);
    
    Serial.println("Initialising UI");
    systemFunctions[SysFuncLedStart] = LedStart;
    systemFunctions[SysFuncLedStop] = LedStop;
    systemFunctions[SysFuncMotorStart] = MotorStart;
    systemFunctions[SysFuncMotorStop] = MotorStop;
    systemFunctions[SysFuncUiSelectBeep] = SpeakerFinishedBeep;

    UiObj = CreateUi(&_settings, systemFunctions);  
}

void LedStop()
{
    Serial.println("Stop UV LED");
    digitalWrite(UVLED, LOW);
    if(!_uvLedOnTimer.IsCompleted())
    {
        _uvLedOnTimer.Stop();
    }

    _motorController.stop();

    SpeakerFinishedBeep();
}

void LedStart()
{
    Serial.println("Start UV LED");    
    digitalWrite(UVLED, HIGH);
    _uvLedOnTimer.SetTimerLength(_settings.on_time*ms);
    _uvLedOnTimer.Start();

    _motorController.setSpeed(_settings.motor_speed);
    _motorController.start();
}

void MotorStart()
{}
void MotorStop()
{}

void SpeakerFinishedBeep()
{
    _soundController.playTone(2000, 2000);
}

void loop() 
{
    
    UiUpdate(UiObj);
    //update_LED();
}
