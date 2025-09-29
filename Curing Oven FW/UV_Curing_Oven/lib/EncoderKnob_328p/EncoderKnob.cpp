#include "EncoderKnob.h"
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define KNOB_DEBOUCE_TIME 10 //ms

#define PORT_B_MAP_OFFSET 0
#define PORT_C_MAP_OFFSET 8
#define PORT_D_MAP_OFFSET 16

/*** Local variables used for interrupts ***/
// Map for registering Encoderknobs to Interrupt pins
// Map is the size of 3 ports, and is split into groups 
// by port in order of {PORTB, PORTC, PORTD}
EncoderKnob* PcIntEncoderKnobMap[24] = {nullptr};

// Bitmap for which pins of port B are registered to an encoder knob
uint8_t PortBPinsRegistered = 0;
// Bitmap for which pins of port C are registered to an encoder knob
uint8_t PortCPinsRegistered = 0;
// Bitmap for which pins of port D are registered to an encoder knob
uint8_t PortDPinsRegistered = 0;

// Saved state of PINB Register from last interrupt
volatile uint8_t PortBLastInputState = 0;
// Saved state of PINC Register from last interrupt
volatile uint8_t PortCLastInputState = 0;
// Saved state of PIND Register from last interrupt
volatile uint8_t PortDLastInputState = 0;

/// @brief Calls service pin interupt function within the encoder knob mapped to the pin that triggered this interrupt
/// @param portOffset map index offset of the port that this interrupt is from
/// @param triggerPin port mask of the pin(s) that triggered this interrupt
void ServicePcInterrupt(uint8_t portOffset, uint8_t triggerPin);

/*ISRs*/

// Interrupt service routine for Port B
ISR (PCINT0_vect)
{
    // Get the new state of all input pins on port b
    uint8_t pinB = PINB;

    // Using Xor, find which pins have changed since last 
    // interrupt and mask by the pins registered to encoderknob objects
    uint8_t triggerPin = (pinB ^ PortBLastInputState) & PortBPinsRegistered;

    // Save new portB input state for next interupt
    PortBLastInputState = pinB;

    // Call pin service method of the EncoderKnob registered to this pin
    ServicePcInterrupt(PORT_B_MAP_OFFSET, triggerPin);
}

// Interrupt service routine for Port C
ISR (PCINT1_vect)
{
    // Get the new state of all input pins on port C
    uint8_t pinC = PINC;

    // Using Xor, find which pins have changed since last 
    // interrupt and mask by the pins registered to encoderknob objects
    uint8_t triggerPin = (pinC ^ PortCLastInputState) & PortCPinsRegistered;

    // Save new portC input state for next interupt
    PortCLastInputState = pinC;

    // Call pin service method of the EncoderKnob registered to this pin
    ServicePcInterrupt(PORT_C_MAP_OFFSET, triggerPin);
}

// Interrupt service routine for Port D
ISR (PCINT2_vect)
{
    // Get the new state of all input pins on port D
    uint8_t pinD = PIND;

    // Using Xor, find which pins have changed since last 
    // interrupt and mask by the pins registered to encoderknob objects
    uint8_t triggerPin = (pinD ^ PortDLastInputState) & PortDPinsRegistered;

    // Save new portD input state for next interupt
    PortDLastInputState = pinD;

    // Call pin service method of the EncoderKnob registered to this pin
    ServicePcInterrupt(PORT_D_MAP_OFFSET, triggerPin);
}

/// @brief Loop through the mask triggerPin, and call ServicePinInterrupt of the EncoderKnob that 
/// sits at the map location of the index of any 1's in triggerPin
void ServicePcInterrupt(uint8_t portOffset, uint8_t triggerPin)
{
    for(int i=0; i < 8 ;i++)
    {
        if(triggerPin & (1<<i))
        {
            // Ensure that the encoderKnob at this index is registered
            if(PcIntEncoderKnobMap[i+portOffset] != nullptr)
            {
                PcIntEncoderKnobMap[i+portOffset]->ServicePinInterrupt(i+portOffset);
            }
        }
    }
}


/*** Public Methods ***/

/// @brief Encoder knob constructor.
/// Save the private parameters, and construct the debounce timer used for knob presses 
EncoderKnob::EncoderKnob(int s1, int s2, int key):
_knobDebounceTimer(KNOB_DEBOUCE_TIME, KnobDebounceCallback_helper, this)
{
    _s1 = s1;
    _s2 = s2;
    _key = key;
}

/// @brief Destructor. Removes pointer registration so we are never left with hanging pointers
EncoderKnob::~EncoderKnob()
{
    DeregisterPin(_s1MapIndex);
    DeregisterPin(_s2MapIndex);
    DeregisterPin(_keyMapIndex);
}

/// @brief Saves the pointer of this encoder knob to the map at each pin index, Initialises each pin, 
// initialises last input state variables, and saves s1 current state
void EncoderKnob::Register()
{
    _s1MapIndex = RegisterPin(_s1);
    _s2MapIndex = RegisterPin(_s2);
    _keyMapIndex = RegisterPin(_key);

    PortBLastInputState = PINB;
    PortCLastInputState = PINC;
    PortDLastInputState = PIND;

    _lastStateS1 = digitalRead(_s1);
}


/// @brief Save callback as private variable
void EncoderKnob::RegisterCWCallback(EncoderCallbackAction callback, void* object)
{
    _cwCallback = callback;
    _cwCallbackContext = object;
}

/// @brief Save callback as private variable
void EncoderKnob::RegisterACWCallback(EncoderCallbackAction callback, void* object)
{
    _acwCallback = callback;
    _acwCallbackContext = object;
}

/// @brief Save callback as private variable
void EncoderKnob::RegisterPressCallback(EncoderCallbackAction callback, void* object)
{
    _pressCallback = callback;
    _pressCallbackContext = object;
}

/// @brief Save callback as private variable
void EncoderKnob::RegisterReleaseCallback(EncoderCallbackAction callback, void* object)
{
    _releaseCallback = callback;
    _releaseCallbackContext = object;
}

/// @brief set the pressed flag back to false, and return its old value
bool EncoderKnob::IsKnobPressed()
{
    bool value = _knobPressedFlag;
    _knobPressedFlag = false;
    return value;
}

/// @brief set the released flag back to false, and return its old value
bool EncoderKnob::IsKnobReleased()
{
    bool value = _knobReleasedFlag;
    _knobReleasedFlag = false;
    return value;
}

/// @brief set the acw rotation flag back to false, and return its old value
bool EncoderKnob::IsEncoderTurnedACW()
{
    bool value = _acwRotationFlag;
    _acwRotationFlag = false;
    return value;
}

/// @brief set the cw rotation flag back to false, and return its old value
bool EncoderKnob::IsEncoderTurnedCW()
{
    bool value = _cwRotationFlag;
    _cwRotationFlag = false;
    return value;
}

/*** Private Methods ***/

/// @brief Find the GPIO port relating to this pin, enable pin change interrupts for the pin, 
/// register this encoder knob to the static interrupt map, and return this pins map index
int EncoderKnob::RegisterPin(uint8_t pin)
{
    // Find the port and bitmask for this pin
	uint8_t bitMask = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);

    // If pin is not a valid pin, return -1 to indicate
    if (port == NOT_A_PIN) return -1;
    if (bitMask == 0) return -1;

    // Get the address of the port register to find which port we are looking at
    volatile uint8_t* portReg = portOutputRegister(port);
    int mapOffset = 0;

    if(portReg == &PORTB)
    {
        // Enable pin change interrupts for Port B (PCIE0)
        PCICR |= (1<<PCIE0);

        // Enable pin change interrupts for this specific pin
        PCMSK0 |= bitMask; 

        // Set Port registered mask with this bit
        PortBPinsRegistered |= bitMask;

        // Set Map offset for use later in this function
        mapOffset = PORT_B_MAP_OFFSET;
    }
    else if (portReg == &PORTC)
    {
        // Enable pin change interrupts for Port C (PCIE1)
        PCICR |= (1<<PCIE1);

        // Enable pin change interrupts for this specific pin
        PCMSK1 |= bitMask; 

        // Set Port registered mask with this bit
        PortCPinsRegistered |= bitMask;

        // Set Map offset for use later in this function
        mapOffset = PORT_C_MAP_OFFSET;
    }
    else if (portReg == &PORTD)
    {
        // Enable pin change interrupts for Port D (PCIE2)
        PCICR |= (1<<PCIE2);

        // Enable pin change interrupts for this specific pin
        PCMSK2 |= bitMask; 

        // Set Port registered mask with this bit
        PortDPinsRegistered |= bitMask;

        // Set Map offset for use later in this function
        mapOffset = PORT_D_MAP_OFFSET;
    }
    else
    {
        // If this pin is not of port B C or D, then return index -1
        return -1;
    }

    // Calculate the bit index of this pin by looping through the bit 
    // mask and finding the index of the first 1
    int bitIndex = 0;
    for(int i = 0; i < 8; i++)
    {
        if(bitMask & (1<<i))
        {
            bitIndex = i;
            break;
        }
    }

    // Save this encoder to the interrupt map at the index of this pin
    PcIntEncoderKnobMap[bitIndex + mapOffset] = this;

    // Set pin to be an input
    pinMode(pin, INPUT);

    // Return interrupt map index
    return bitIndex + mapOffset;
}

/// @brief remove any registration of this pin in the map variables used for interrupts
void EncoderKnob::DeregisterPin(uint8_t mapIndex)
{
    // If map index is -1, then the pin was never registered and we may return
    if(mapIndex == -1)
    {
        return;
    }

    // remove EncoderKnob reference from map for this pin
    PcIntEncoderKnobMap[mapIndex] = nullptr;

    // Disable interrupt for this pin, and remove from port registered mask
    if(mapIndex < (PORT_B_MAP_OFFSET + 8))
    {
        uint8_t bitMask = (1 << (mapIndex - PORT_B_MAP_OFFSET));

        PCMSK0 &= ~bitMask; 
        PortBPinsRegistered &= ~bitMask;
    } 
    else if (mapIndex < (PORT_C_MAP_OFFSET + 8))
    {
        uint8_t bitMask = (1 << (mapIndex - PORT_B_MAP_OFFSET));
        
        PCMSK1 &= ~bitMask; 
        PortCPinsRegistered &= ~bitMask;
    } 
    else if (mapIndex < (PORT_D_MAP_OFFSET + 8))
    {
        uint8_t bitMask = (1 << (mapIndex - PORT_B_MAP_OFFSET));
        
        PCMSK2 &= ~bitMask; 
        PortCPinsRegistered &= ~bitMask;
    }
}

/// @brief If pin interrupt was an encoder, calls encoder ISR.
// If interrupt was knob, starts the debounce timer.
void EncoderKnob::ServicePinInterrupt(uint8_t mapIndex)
{
    if(mapIndex == _s1MapIndex ||
        mapIndex == _s2MapIndex)
    {
        ServiceEncoderInterrupt();
    }
    else if (mapIndex == _keyMapIndex)
    {
        _knobDebounceTimer.Start();
    } 
}

/// @brief Service the knob interrupt by setting the appropriate flags and running callbacks 
void EncoderKnob::KnobDebounceCallback()
{
    if(digitalRead(_key) == LOW)
    {
        // Falling Edge (Press) Interrupt
        _knobPressedFlag = true;

        if(_pressCallback != nullptr)
        {
            _pressCallback(_pressCallbackContext);
        }
    }
    else
    {
        // Rising Edge (Release) Interrupt
        _knobReleasedFlag = true;

        if(_releaseCallback != nullptr)
        {
            _releaseCallback(_releaseCallbackContext);
        }
    }
}

/// @brief Service the encoder turn interrupt by checking the current and previous 
/// states of S1 and S2 and calculating if it was a CW or ACW rotation.
/// Set the appropriate flags and run callbacks dependant on rotation result.
void EncoderKnob::ServiceEncoderInterrupt()
{
    bool stateS1 = digitalRead(_s1);
    // If last and current state of S1 are different, then pulse occurred
    // React to only 1 state change to avoid double count
    if (stateS1 != _lastStateS1  && stateS1 == 1)
    {
        // If the S1 state is different than the S2 state then
        // the encoder is rotating CW 
        if (digitalRead(_s2) != stateS1) {
          
            _cwRotationFlag = true;

            if(_cwCallback != nullptr)
            {
                _cwCallback(_cwCallbackContext);
            }

        } else {
            // Encoder is rotating ACW
            _acwRotationFlag = true;
            
            if(_acwCallback != nullptr)
            {
                _acwCallback(_acwCallbackContext);
            }
        }
    }
    // Remember last S1 state
    _lastStateS1 = stateS1;
}



