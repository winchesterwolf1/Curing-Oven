#ifndef UVLAMP_H
#define UVLAMP_H

/// @brief Class for controlling the UV LED state
class UvLamp {
    public:

        /// @brief Constructor for the LED class
        /// @param pin digital output pin to use for controlling the UV light
        UvLamp(int pin);

        /// @brief Sets up the Uv lamp object ready for use
        void Setup();

        /// @brief Switch Led on
        void On();

        /// @brief Switch Led off
        void Off();

        /// @brief Gets the current state of the UV LED
        /// @return true of LED is on
        bool IsOn();

    private:

        /// @brief Pin to use to switch LED on and off
        int _pin;

        /// @brief Current state of the LED
        bool _state;
};

#endif // UVLAMP_H