#include "Motor.h"
#include "Arduino.h"

#define U8BIT_TO_PERCENT(P) ( (P * 100 ) / UINT8_MAX )
#define PERCENT_TO_U8BIT(P) ( (P * UINT8_MAX ) / 100 )

Motor::Motor(int pin) 
{
    if(!digitalPinHasPWM(pin))
    {
        return;
    }

    _pwmPin = pin;
    pinMode(_pwmPin, OUTPUT);
    digitalWrite(_pwmPin, LOW);
    _speed = 0;
}

Motor::~Motor()
{
    pinMode(_pwmPin, INPUT);
}

void Motor::setSpeed(unsigned int speed) 
{
    if(speed > 100)
    {
        speed = 100;
    }

    _speed = PERCENT_TO_U8BIT(speed);
}

int Motor::getSpeed() 
{
    return U8BIT_TO_PERCENT(_speed);
}

void Motor::start() 
{
    Serial.println(_pwmPin);
    analogWrite(_pwmPin, _speed);
}

void Motor::stop() 
{
    digitalWrite(_pwmPin, LOW);
}
