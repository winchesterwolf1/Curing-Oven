#include "UvLamp.h"
#include "Arduino.h"

/// @brief Save pin for later use
UvLamp::UvLamp(int pin) :
_pin(pin)
{
}

/// @brief Initialise the pin as an output. using Arduino libraries for now
void UvLamp::Setup()
{
    pinMode(_pin, OUTPUT);
}

/// @brief Turn the Uv Led On. Using arduino Libraries for now
void UvLamp::On()
{
    digitalWrite(_pin, HIGH);
    _state = true;
}

/// @brief Turn the Uv Led Off. Using arduino Libraries for now
void UvLamp::Off()
{
    digitalWrite(_pin, LOW);
    _state = false;
}

/// @brief simply return the internal variable
bool UvLamp::IsOn()
{
    return _state;
}