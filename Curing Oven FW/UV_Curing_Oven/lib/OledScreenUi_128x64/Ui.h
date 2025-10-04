#include "Screens/Screen.h"
#include "Screens/NumericInput.h"
#include "EncoderKnob.h"
#include "Screens/Menu.h"
#include "ToneGenerator.h"

#ifndef UI_H
#define UI_H

// Maximum number of screens allowed to be registered to the UI. 
#define MAX_SCREENS 10

/// @brief struct for entries in the screen list 
struct ScreenEntry
{
    const char* name = nullptr;
    Screen* screen = nullptr;
};

/// @brief Class to hold all of the functional Ui elements to handle displaying screens and running callbacks
class Ui 
{
    public:

        /// @brief Method used to draw the current screen to the OLED screen
        void Draw();

        /// @brief Checks for any input from the user and runs callback functions 
        /// @attention Run this as often as possible when not using User Interrupts
        void CheckUserInput();

        /// @brief Runs the Ui callback functions as interrupt driven from the encoderknob
        /// @attention If using this option all callback and system functions must be as small as possible
        void StartUserInterrupts();

    protected:

        /// @brief Constructor for the Ui
        /// @param knobS1 S1 pin of the knob used to control this ui
        /// @param knobS2 S2 pin of the knob used to control this ui
        /// @param knobKey key pin of the knob used to control this ui
        /// @param oledRot Rotation setting of the Oled used to display this ui
        /// @param oledCs Cs pin of the Oled used to display this ui
        /// @param oledDc Dc pin of the Oled used to display this ui
        /// @param oledReset Reset pin of the Oled used to display this ui
        /// @param speaker Speaker pin of this Ui
        Ui(int knobS1, int knobS2, int knobKey, const u8g2_cb_t *oledRot, int oledCs, int oledDc, int oledReset, int speaker);

        /// @brief Adds a screen to the list of screens
        /// @param name name of the screen
        /// @param screen pointer for the screen
        /// @return True if successful. False if there was not enough room in list
        bool AddScreen(const char* name, Screen* screen);
        
        /// @brief Sets the screen of the Ui to a different screen in the list
        /// @param name name of the screen to switch to
        void SetScreen(const char* name);

        /// @brief Action to set the screen back to the previously seen screen.
        void ReturnToPreviousScreen();

        /// @brief Encoder knob object used to get user input
        EncoderKnob _encoderKnob;

        /// @brief Oled object used to draw the screens
        U8G2_SH1106_128X64_NONAME_1_4W_HW_SPI _u8g2;

        /// @brief Controller for producing audio feecback to the user
        ToneGenerator _toneGenerator;

    private:

        /// @brief List of screens used to draw the Ui
        ScreenEntry _screens[MAX_SCREENS];
        /// @brief Number of entries in _screens
        uint8_t _screenCount = 0;
        /// @brief Pointer to the currently shown screen
        Screen* _currentScreen = nullptr;
        /// @brief Pointer to the previous screen
        Screen* _previousScreen = nullptr;

        /// @brief Callback function for when the encoder knob is turned CW
        void handleEncoderCW();
        /// @brief static helper function for Cw Turn callback
        static void encoderCWCallback_helper(void* object) {
            static_cast<Ui*>(object)->handleEncoderCW();
        }

        /// @brief Callback function for when the encoder knob is turned ACW
        void handleEncoderACW();
        /// @brief static helper function for Acw Turn callback
        static void encoderACWCallback_helper(void* object) {
            static_cast<Ui*>(object)->handleEncoderACW();
        }

        /// @brief Callback fuction for when the encoder knob is pressed
        void handleKnobPress();
        /// @brief static helper function for Press callback
        static void knobPressCallback_helper(void* object) {
            static_cast<Ui*>(object)->handleKnobPress();
        }

        /// @brief Callback function for when the encoder knob is released
        void handleKnobRelease();
        /// @brief static helper function for Release callback
        static void knobReleaseCallback_helper(void* object) {
            static_cast<Ui*>(object)->handleKnobRelease();
        }
};

#endif /*UI_H*/
