#include "Ui.h"
#include "EncoderKnob.h"

#define KNOB_PRESS_BEEP_FREQUENCY 1000
#define KNOB_PRESS_BEEP_TIME 40


/// @brief Initialise Encoder Knob and Oled screen, then register callback functions, and start the Ui.
Ui::Ui(int knobS1, int knobS2, int knobKey, const u8g2_cb_t *oledRot, int oledCs, int oledDc, int oledReset, int speaker):
_encoderKnob(knobS1, knobS2, knobKey), 
_u8g2(oledRot, oledCs, oledDc, oledReset),
_toneGenerator(speaker)
{    
    _encoderKnob.Register();

    _u8g2.begin();
    _u8g2.setFlipMode(1);

}

/// @brief draw the current screen
void Ui::Draw()
{
    _currentScreen->DrawScreen();
}

/// @brief Check for each of the user inputs and service the screen apropriately
void Ui::CheckUserInput()
{
    if(_encoderKnob.IsKnobPressed())
    {
        handleKnobPress();
    }

    if(_encoderKnob.IsKnobReleased())
    {
        handleKnobRelease();
    }

    if(_encoderKnob.IsEncoderTurnedACW())
    {
        handleEncoderACW();
    }

    if(_encoderKnob.IsEncoderTurnedCW())
    {
        handleEncoderCW();
    }
}

/// @brief Register the callback helpers for each user input to the encoder knob
void Ui::StartUserInterrupts()
{
    _encoderKnob.RegisterCWCallback(encoderCWCallback_helper, this);
    _encoderKnob.RegisterACWCallback(encoderACWCallback_helper, this);
    _encoderKnob.RegisterReleaseCallback(knobReleaseCallback_helper, this);
    _encoderKnob.RegisterPressCallback(knobPressCallback_helper, this);
}

/// @brief if there is room in the array set the next available entry to the new name and screen pointer
bool Ui::AddScreen(const char* name, Screen* screen)
{
    if (_screenCount >= MAX_SCREENS) {
        return false; // Indicate failure
    }
    _screens[_screenCount].name = name;
    _screens[_screenCount].screen = screen;
    _screenCount++;

    return true;
}

/// @brief search through the screen entries for the entered name. 
/// If found set the current and previous screens apropriately
void Ui::SetScreen(const char* name)
{
    for(int i = 0; i < MAX_SCREENS; i++)
    {
        if(strcmp(_screens[i].name, name) == 0)
        {
            if(_screens[i].screen != nullptr)
            {
                _previousScreen = _currentScreen;
                _currentScreen = _screens[i].screen;
            }
        }
    }
}

/// @brief simply set the current screen equal to the previous screen
void Ui::ReturnToPreviousScreen()
{
    _currentScreen = _previousScreen;
}

/// @brief call the current screens callback function
void Ui::handleEncoderCW()
{
    _currentScreen->OnCwRotation();
}

/// @brief call the current screens callback function
void Ui::handleEncoderACW()
{
    _currentScreen->OnAcwRotation();
}

/// @brief Do nothing, only do actions on release
void Ui::handleKnobPress()
{

}

/// @brief call the current screens callback function
void Ui::handleKnobRelease()
{
    _currentScreen->OnButtonClick();

    _toneGenerator.playTone(KNOB_PRESS_BEEP_FREQUENCY, KNOB_PRESS_BEEP_TIME);
}




