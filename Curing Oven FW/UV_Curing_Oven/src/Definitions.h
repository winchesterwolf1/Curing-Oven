#ifndef DEFINITIONS_H // include guard
#define DEFINITIONS_H

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define UVLED 5

#define KNOB_PUSH 2 //PD2, D2, Push switch Active Low
#define KNOB_ENC_1 15 //PC1, A1/D15. ACW rotation second low
#define KNOB_ENC_2 14 //PC0, A0/D14. ACW rotation first low
#define CW_rot 1

#define SCREEN_REFRESH_RATE 100 //ms

#define UVLED_DEFAULT_ON_TIME 1
#define DEFAULT_MOTOR_SPEED 100

#define ms 1000

struct SelectedSettings {
    int on_time = UVLED_DEFAULT_ON_TIME;
    int brightness = 100;
    int motor_speed = DEFAULT_MOTOR_SPEED;
};

enum SysFuncIndex
{
    SysFuncLedStart,
    SysFuncLedStop,
    SysFuncMotorStart,
    SysFuncMotorStop
};

/* Debugging Defines */
//#define TIMESERVER_DEBUG


#endif /* DEFINITIONS_H */