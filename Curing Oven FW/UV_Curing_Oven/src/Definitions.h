#ifndef DEFINITIONS_H // include guard
#define DEFINITIONS_H

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "PseudoRtos.h"

/*** Digital Pin Mapping ***/
// Digital input pin mapped to the UVLED pin. PD5, D5, Active High
#define UVLED 5
// Digital input pin mapped to the Motor pin. PD9, D9, Active High
#define MOTOR_EN_PIN 9
// Digital input pin mapped to the Speaker pin. PD3, D3, Active High
#define SPEAKER_PIN 3
// Digital input pin mapped to the Knob key pin. PD2, D2, Push switch Active Low
#define KNOB_PUSH 2
// Digital input pin mapped to the Knob S1 pin. PC1, A1/D15. ACW rotation second low
#define KNOB_ENC_1 14
// Digital input pin mapped to the Knob S2 pin. PC0, A0/D14. ACW rotation first low
#define KNOB_ENC_2 15
// SPI Chip Select Pin used for controlling the OLED screen
#define OLED_CS_PIN 10
// SPI Data/Command pin used for controlling the OLED screen
#define OLED_DC_PIN 8
// Reset pin used for controlling the OLED screen
#define OLED_RESET_PIN 7

/*** System Parameters ***/
// Number of Ms delay between each screen draw
#define SCREEN_REFRESH_RATE 100
// Default Setting for the curing time
#define UVLED_DEFAULT_ON_TIME 1
// Default Setting for the motor speed
#define DEFAULT_MOTOR_SPEED 100
// Length of the system event queue
#define SYSTEM_EVENT_QUEUE_LENGTH 10


/*** Conversion Parameters ***/
// Seconds to Milliseconds conversion
#define ms 1000


/*** Global Classes, Structs and Enums and types ***/

/// @brief Settings class for the system settings
struct SelectedSettings {
    int CuringTime = UVLED_DEFAULT_ON_TIME;
    int brightness = 100;
    int MotorSpeed = DEFAULT_MOTOR_SPEED;
};

/// @brief System events that can be queued in the system event queue
typedef enum
{
    SysEvt_StartCuringPressed,
    SysEvt_CuringTimerCompleted,
    SysEvt_CancelCuringPressed
} SysEvt_t;

/// @brief Type for the system event queue
typedef Queue<SysEvt_t, SYSTEM_EVENT_QUEUE_LENGTH> SysEvtQueue;

/// @brief Enum to list all of the available queues
enum ApplicationQueues
{
    Queues_SysEvt = 0,
};

/// @brief Enum to list all of the available semaphores
enum ApplicationSemaphores
{
    Sem_LampOn = 0,
    Sem_LampOff,
    Sem_MotorOn,
    Sem_MotorOff,
    Sem_UiNotifyCompleted,
    Sem_StartCuringTimer,
    Sem_CancelCuringTimer,
};

/// @brief Enum to list all of the available mutexes
enum ApplicationMutexes
{
};

/// @brief Enum to list all of the available counting semaphores
enum ApplicationCountSems
{
};

/*** PseudoRTOS Memory Settings ***/
#undef SEMAPHORE_ARRAY_LEN
#define SEMAPHORE_ARRAY_LEN 10
// Maximum of items to store in the queue array
#undef QUEUE_ARRAY_LEN
#define QUEUE_ARRAY_LEN 1
// Maximum of items to store in the mutex array
#undef MUTEX_ARRAY_LEN
#define MUTEX_ARRAY_LEN 0
// Maximum of items to store in the counting semaphore array
#undef COUNTSEMAPHORE_ARRAY_LEN
#define COUNTSEMAPHORE_ARRAY_LEN 0

/*** Debugging Defines ***/
//#define TIMESERVER_DEBUG


#endif /* DEFINITIONS_H */