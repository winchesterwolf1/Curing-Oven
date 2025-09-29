#include "Timer.h"
#include <stdint.h>

#ifndef KNOB_H // include guard
#define KNOB_H

// A generic C-style callback function pointer that takes a void* context
typedef void (*EncoderCallbackAction)(void* object);

class EncoderKnob
{
    private:
        /// @brief Pin to use for S1
        int _s1;
        /// @brief Interrupt map index for S1
        int _s1MapIndex;

        /// @brief Pin to use for S2
        int _s2;
        /// @brief Interrupt map index for S2
        int _s2MapIndex;

        /// @brief Pin to use for Key
        int _key;
        /// @brief Interrupt map index for key
        int _keyMapIndex;

        /// @brief Internal flag to denote if the knob was pressed since last poll
        bool _knobPressedFlag = false;

        /// @brief Internal flag to denote if the knob was released since last poll
        bool _knobReleasedFlag = false;

        /// @brief Internal flag to indicate if a CW rotation event occurred since last poll
        bool _cwRotationFlag = false;

        /// @brief Internal flag to indicate if an ACW rotation even occurred since last poll
        bool _acwRotationFlag = false;

        /// @brief current state of S1 to help in calculation of CW or ACW rotations
        int currentStateS1;
        /// @brief last state of S1 to help in calculation of CW or ACW rotations
        int _lastStateS1;

        /// @brief clockwise callback function pointer
        EncoderCallbackAction _cwCallback = nullptr;
        /// @brief clockwise callback function context
        void* _cwCallbackContext = nullptr;
        /// @brief anti-clockwise callback function pointer
        EncoderCallbackAction _acwCallback = nullptr;
        /// @brief anti-clockwise callback function context
        void* _acwCallbackContext = nullptr;
        /// @brief knob press callback function pointer
        EncoderCallbackAction _pressCallback = nullptr;
        /// @brief knob press callback function context
        void* _pressCallbackContext = nullptr;
        /// @brief knob release callback function pointer
        EncoderCallbackAction _releaseCallback = nullptr;
        /// @brief knob release callback function context
        void* _releaseCallbackContext = nullptr;

        /// @brief Timer used to debounce the encoder knob presses. Should be started on an knob interrupt
        Timer _knobDebounceTimer;

        /// @brief Callback functon for debouncing timer which services a knob press
        void KnobDebounceCallback();

        // Static helper function for debounce callbacks
        static void KnobDebounceCallback_helper(void* object) {
            static_cast<EncoderKnob*>(object)->KnobDebounceCallback();
        }

        /// @brief Method to call when an interrupt occurrs on one of the pins associated 
        /// with this EncoderKnob. Finds if the interrupt is for Encoder turn of Knob press
        /// @param mapIndex Interrupt map index of the pin that triggered this interrupt
        void ServicePinInterrupt(uint8_t mapIndex);

        /// @brief Method to call on encoder interrupt
        void ServiceEncoderInterrupt();

        /// @brief Initialises and Registers a pin to the encoderknob interrupt map
        /// @param pin pin to register
        /// @return interrupt map index of this pin
        int RegisterPin(uint8_t pin);

        /// @brief deregisters this pin from any interrupt maps
        /// @param mapIndex map index of the pin to deregister
        void DeregisterPin(uint8_t mapIndex);

        // Friend functions to allow encoder params to be accessed
        friend void ServicePcInterrupt(uint8_t portOffset, uint8_t triggerPin);

    public:
        /// @brief Constructor for encoder knob
        /// @param S1 Pin for S1
        /// @param S2 Pin for S2
        /// @param Key Pin for Key
        EncoderKnob(int s1, int s2, int key);

        /// @brief Destructor
        ~EncoderKnob();

        /// @brief Registers the Encoder Knob object to its ISRs
        void Register();

        /// @brief Register callback function to be called on a clockwise rotation
        /// @param callback 
        /// @param object
        void RegisterCWCallback(EncoderCallbackAction callback, void* object);

        /// @brief Register callback function to be called on an anti clockwise rotation
        /// @param callback 
        /// @param object
        void RegisterACWCallback(EncoderCallbackAction callback, void* object);

        /// @brief Register callback function for a press interrupt
        /// @param callback 
        /// @param object
        void RegisterPressCallback(EncoderCallbackAction callback, void* object);

        /// @brief Register callback function for a release interrupt
        /// @param callback 
        /// @param object
        void RegisterReleaseCallback(EncoderCallbackAction callback, void* object);

        /// @brief Check if knob was pressed since last checked
        /// @return true if knob was pressed since this function last ran
        bool IsKnobPressed();

        /// @brief Check if knob was released since last checked
        /// @return true if knob was released since this function last ran
        bool IsKnobReleased();

        /// @brief Check if anti-clockwise occurred since last checked
        /// @return true if ACW rotation logged since this function last ran
        bool IsEncoderTurnedACW();

        /// @brief Check if clockwise occurred since last checked
        /// @return true if CW rotation logged since this function last ran
        bool IsEncoderTurnedCW();
};

#endif /* KNOB_H */