#include "NumericInput.h"

// Y index of the setting parameter that is being drawn
#define PARAMETER_DRAW_HEIGHT 40

// Text on the button
#define BUTTON_TEXT "OK"
// Y index of the button on the screeb
#define BUTTON_Y 60
// Width of the button
#define BUTTON_WIDTH 45
// Horizontal padding to the button border (not really used unless text is long)
#define BUTTON_HORIZ_PADDING 10
// Vertical padding to the button border
#define BUTTON_VERT_PADDING 2
// Button draw style
#define BUTTON_STYLE (U8G2_BTN_INV | U8G2_BTN_BW1 | U8G2_BTN_HCENTER)

/// @brief constructor, simply initialise screen with references to this object
NumericInput::NumericInput(): Screen(&_oled, _bannerText)
{ }

/// @brief Sets the info and references on the screen
void NumericInput::Set(U8G2* oled, const char* bannerText, int* outputParameter, int minValue, int maxValue, UiCallback action, void* actionObject)
{
    _oled = oled;
    strncpy(_bannerText, bannerText, SCREEN_CHARACTER_LIMIT);
    _outputParameter = outputParameter;
    sprintf(_outputParameterStr, "%d", *_outputParameter);
    _minValue = minValue;
    _maxValue = maxValue;
    _action = action;
    _actionObject = actionObject;
}

/// @brief Data on the screen is designed as the value of the output parameter in the centre with an OK button at the bottom of the screen
void NumericInput::DrawData()
{
    // Draw number in the centre of the screen
    _oled->drawStr(((_oled->getDisplayWidth()-_oled->getStrWidth(_outputParameterStr))/2), PARAMETER_DRAW_HEIGHT, _outputParameterStr);

    // Draw button
    _oled->drawButtonUTF8(_oled->getDisplayWidth()/2, BUTTON_Y, BUTTON_STYLE, BUTTON_WIDTH, 
        BUTTON_HORIZ_PADDING, BUTTON_VERT_PADDING, BUTTON_TEXT );
}

/// @brief Increment the output parameter if it is not at the max value
void NumericInput::OnCwRotation()
{
    if((*_outputParameter) < _maxValue)
    {
        (*_outputParameter)++;
    }

    sprintf(_outputParameterStr, "%d", (*_outputParameter));
}

/// @brief Decrement the output parameter if it is not at the min value
void NumericInput::OnAcwRotation()
{
    if((*_outputParameter) > _minValue)
    {
        (*_outputParameter)--;
    }

    // save new value to a string for drawing
    sprintf(_outputParameterStr, "%d", (*_outputParameter));
}

/// @brief runs the action registered to button click for this page
void NumericInput::OnButtonClick()
{
    // Check values are valid before running the callback action
    if (_action != nullptr && _actionObject != nullptr) {
            _action(_actionObject);
    }
}


