#ifndef TONE_GENERATOR_H
#define TONE_GENERATOR_H

/// @brief Class to be used to control a speaker output for creating tones
class ToneGenerator {
    public:

        /// @brief Constructor for the tone generator class
        /// @param pin digital pin to output the tone to
        ToneGenerator(int pin);

        /// @brief Class destructor
        ~ToneGenerator();

        /// @brief Initialise output pin and set registers
        void Setup();

        /// @brief Plays a frequency on the output pin of specified frequency and duration
        /// @attention If duration is set to 0, output will stay on untill stopped
        /// @param frequency Frequency of the output tone
        /// @param duration Duration of the output tone. 0 will output forever.
        void playTone(int frequency, int duration);

        /// @brief Stops the current tone output
        void stop();

    private:    

        /// @brief Output pin for tones.
        int _tonePin;
};

#endif // TONE_GENERATOR_H