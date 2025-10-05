#include "Ui.h"
#include "Definitions.h"

#ifndef UV_CURING_OVEN_UI_H
#define UV_CURING_OVEN_UI_H

/// @brief Implementation of the UV curing oven UI
class UvCuringOvenUi : public Ui
{
    public:

    /// @brief Constructor for this UI implementation
    /// @param knobS1 Knob S1 pin
    /// @param knobS2 Knob S2 pin
    /// @param knobKey Knob Key pin
    /// @param oledRot Oled Rotation selection
    /// @param oledCs Oled Cs Pin
    /// @param oledDc Oled Dc pin
    /// @param oledReset Oled Reset Pin
    /// @param selectedSettings pointer to the system settings that will be updated by the Ui
    /// @param systemFunctions pointer to the system callback functions that will be run by the Ui
    UvCuringOvenUi(
        int knobS1, int knobS2, int knobKey, 
        const u8g2_cb_t *oledRot, int oledCs, int oledDc, int oledReset,
        int speaker,
        SelectedSettings* selectedSettings, QueueBase* evtQ);

    /// @brief Intialise all pins and registers relating to this Ui and start communications
    void Setup();

    /// @brief Initialises all the main menu items and callbacks, stores them 
    /// all in a list, initialises the main menu, saves the main menu to the Ui, 
    /// and sets the Main menu as the current screen
    void SetupMainMenu();

    /// @brief Initialises the re-usable numeric input screen, and adds it to the UI
    void SetupNumericInput();

    /// @brief Sets the main menu status based on the current state of the UVLED pin
    /// @attention this should be run as often as possible
    void UpdateMainMenuStatus();

    /// @brief Plays a tone to the user indicating that curing has completed
    void PlayCureCompleteBeep();

    private:

    /* Timers */

    /// @brief Timer for indicating to the user that the curing has completed
    Timer _cureCompleteBeepTimer;

    /// @brief Callback wrapper for curecomplete timer
    /// @param ui ui object to call beep from
    static void CureCompleteBeepCallback(void* ui)
    {
        static_cast<UvCuringOvenUi*>(ui)->PlayCureCompleteBeep();
    }

    /* Screen Objects */

    /// @brief Main Menu screen
    Menu _mainMenu;
    /// @brief Re-usable numeric input screen 
    NumericInput _reusableNumericInput;


    /* Settings objects and system functions to be manipulated by callbacks and pages */

    /// @brief Setting object list to modify in the menu
    SelectedSettings* _selectedSettings;
    /// @brief System Event Queue
    QueueBase* _evtQ;


    /* Callbacks functions for each page and menu button */

    /// @brief Return wrapper for the built in ReturnToPreviousScreen method
    UiCallbackWrapper<UvCuringOvenUi> _returnWrapper;

    /// @brief Callback function for the UV On button in the main menu
    void UvOnAction();
   
    /// @brief Callback function for the UV Off button in the main menu
    void UvOffAction();

    /// @brief Callback function for the Set Time button in the main menu
    void SetTimeAction();
    
    /// @brief Callback function for the UV Brightness button in the main menu
    void UvBrightnessAction();

    /// @brief Callback function for the Motor Speed button in the main menu
    void MotorSpeedAction();

    /// @brief Callback function for the useless button in the main menu
    void UselessAction();
};

#endif // UV_CURING_OVEN_UI_H