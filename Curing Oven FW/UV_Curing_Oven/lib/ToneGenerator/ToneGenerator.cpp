#include "ToneGenerator.h"
#include "Arduino.h"

/// @brief Save pin to class
ToneGenerator::ToneGenerator(int pin) :
_tonePin(pin)
{ }

/// @brief Default implementation atm
ToneGenerator::~ToneGenerator()
{

}

/// @brief Use arduino implmenetation to create tone atm
void ToneGenerator::playTone(int frequency, int duration)
{
    tone(_tonePin, frequency, duration);
}

/// @brief Not yet implmeneted
void ToneGenerator::stop()
{

}