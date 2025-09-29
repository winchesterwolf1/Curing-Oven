#include "Sound.h"
#include "Arduino.h"

Sound::Sound(int pin)
{
    _tonePin = pin;
}

Sound::~Sound()
{

}

void Sound::playTone(int frequency, int duration)
{
    tone(_tonePin, frequency, duration);
}

void Sound::stop()
{

}