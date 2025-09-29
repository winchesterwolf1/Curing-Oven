#include "Screen.h"
#include "UiCallbackWrapper.h"
#ifndef NUMERIC_INPUT_H
#define NUMERIC_INPUT_H

/// @brief Class used to draw a numeric input screen and handle parameter updates based on encoder knob input
class NumericInput : public Screen
{
    private:
        /// @brief Text to display at the top of the screen
        char _bannerText[SCREEN_CHARACTER_LIMIT + 1] {0};

        /// @brief pointer to OLED screen to draw
        U8G2* _oled = nullptr;

        /// @brief pointer to the output parameter to increment/decrement
        int* _outputParameter;

        /// @brief converted string value of output parameter
        char _outputParameterStr[5] = {0};

        /// @brief min value of the output parameter
        int _minValue;

        /// @brief max value of the output parameter
        int _maxValue;

        /// @brief action to run when the knob is pressed
        UiCallback _action = nullptr;
        
        /// @brief context for the action to run
        void* _actionObject = nullptr;

        /// @brief Implementation of the draw data method ran by the base screen class when drawing the screen
        void DrawData();
        
    public:
        /// @brief Constructor
        /// @param oled oled to draw to 
        /// @param bannerText banner text of this screen
        /// @param outputParameter output parameter to increment/decrement
        /// @param minValue min value of the output parameter
        /// @param maxValue max value of the output parameter
        NumericInput();
        
        void Set(U8G2* oled, const char* bannerText, int* outputParameter, int minValue, int maxValue, UiCallback action, void* actionObject);

        /// @brief method to call when a CW rotation occurs while on this screen
        void OnCwRotation();

        /// @brief method to call when an ACW rotation occurs while on this screen
        void OnAcwRotation();

        /// @brief method to call when a button click occurs while on this screen
        void OnButtonClick();
};


#endif /*NUMERIC_INPUT_H*/