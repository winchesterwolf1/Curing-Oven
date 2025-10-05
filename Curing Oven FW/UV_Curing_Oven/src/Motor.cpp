#include "Motor.h"
#include "Arduino.h"

// Macro to convert a unsigned 8 bit value to a percentage of 255
#define U8BIT_TO_PERCENT(P) ( (P * 100 ) / UINT8_MAX )
// Macro to convert a percentage to a proportional value on rage of 255
#define PERCENT_TO_U8BIT(P) ( (P * UINT8_MAX ) / 100 )

/// @brief Save default values. Set pwm pin to pin if it is a PWM pin
Motor::Motor(int pin) :
    _speed(0), _pwmPin(0)
{
    if(!digitalPinHasPWM(pin))
    {
        return;
    }

    _pwmPin = pin;
}

/// @brief Destructor, set digital pin to an input/high impedance
Motor::~Motor()
{
    pinMode(_pwmPin, INPUT);
}

/// @brief Initialise PWM pin and set its output value low
void Motor::Setup()
{
    pinMode(_pwmPin, OUTPUT);
    digitalWrite(_pwmPin, LOW);
    _speed = 0;
}

/// @brief convert new speed from a percentage to 8bit value and save. if input is larger than 100 cap to 100
void Motor::setSpeed(unsigned int speed) 
{
    if(speed > 100)
    {
        speed = 100;
    }

    _speed = PERCENT_TO_U8BIT(speed);
}

/// @brief return current speed setting after converting back to percentage
int Motor::getSpeed() 
{
    return U8BIT_TO_PERCENT(_speed);
}

/// @brief start PWM output using arduino atm
void Motor::start() 
{
    analogWrite(_pwmPin, _speed);
}

/// @brief Set the Pwm output pin to low
void Motor::stop() 
{
    digitalWrite(_pwmPin, LOW);
}
