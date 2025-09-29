#ifndef SOUND_H
#define SOUND_H

// Sound.h - Generic header for sound-related functionality

class Sound {
public:
    Sound(int pin);
    ~Sound();

    void playTone(int frequency, int duration);
    void stop();

private:
    int _tonePin;
};

#endif // SOUND_H