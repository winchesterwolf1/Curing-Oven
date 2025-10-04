#include "UiTask.h"
#include "Ui.h"
#include "EventManagerTask.h"
#include "Timer.h"

// Number of items in the main menu
#define MAIN_MENU_NUM_ITEMS 6

#define CURE_COMPLETE_BEEP_FREQ 2000
#define CURE_COMPLETE_BEEP_DURATION 1500
#define CURE_COMPLETE_BEEP_PAUSE_DURATION 1500
#define CURE_COMPLETE_BEEP_NUMBER 3

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
        SelectedSettings* selectedSettings, SysEvtQueue* evtQ)
        :
        Ui(knobS1, knobS2, knobKey, oledRot, oledCs, oledDc, oledReset, speaker),
        _cureCompleteBeepTimer(CURE_COMPLETE_BEEP_DURATION + CURE_COMPLETE_BEEP_PAUSE_DURATION, CureCompleteBeepCallback, this),
        _selectedSettings(selectedSettings),
        _evtQ(evtQ),
        _returnWrapper({this, &UvCuringOvenUi::ReturnToPreviousScreen})
    {
    }

    /// @brief Initialises all the main menu items and callbacks, stores them 
    /// all in a list, initialises the main menu, saves the main menu to the Ui, 
    /// and sets the Main menu as the current screen
    void SetupMainMenu()
    {
        // Static storage for menu item objects
        static MenuItem menuItems[MAIN_MENU_NUM_ITEMS];
        // Static storage for callback wrappers
        static UiCallbackWrapper<UvCuringOvenUi> callbackWrappers[MAIN_MENU_NUM_ITEMS];


        // Initialize the wrappers and menu items
        callbackWrappers[0] = {this, &UvCuringOvenUi::UvOnAction};
        menuItems[0].Set("UV On", callbackWrappers[0].Execute, &callbackWrappers[0]);
        
        callbackWrappers[1] = {this, &UvCuringOvenUi::UvOffAction};
        menuItems[1].Set("UV Off", callbackWrappers[1].Execute, &callbackWrappers[1]);

        callbackWrappers[2] = {this, &UvCuringOvenUi::SetTimeAction};
        menuItems[2].Set("Set Time", callbackWrappers[2].Execute, &callbackWrappers[2]);

        callbackWrappers[3] = {this, &UvCuringOvenUi::UvBrightnessAction};
        menuItems[3].Set("UV Brightness", callbackWrappers[3].Execute, &callbackWrappers[3]);

        callbackWrappers[4] = {this, &UvCuringOvenUi::MotorSpeedAction};
        menuItems[4].Set("Motor Speed", callbackWrappers[4].Execute, &callbackWrappers[4]);
        
        callbackWrappers[5] = {this, &UvCuringOvenUi::UselessAction};
        menuItems[5].Set("Useless", callbackWrappers[5].Execute, &callbackWrappers[5]);
        
        // Configure the main menu screen
        _mainMenu.Set(&_u8g2, "UV LED OFF", MAIN_MENU_NUM_ITEMS, menuItems);
        
        // Add the screen to the UI
        AddScreen("mainMenu", &_mainMenu);
        // Set main menu to be the starting screen
        SetScreen("mainMenu");
    }

    /// @brief Initialises the re-usable numeric input screen, and adds it to the UI
    void SetupNumericInput()
    {
        // Set up the screen for exposure time, just as a placeholder
        _reusableNumericInput.Set(&_u8g2, "Exposure Time", &_selectedSettings->CuringTime, 0, 100, _returnWrapper.Execute, &_returnWrapper);
        AddScreen("numericInput", &_reusableNumericInput);
    }

    /// @brief Sets the main menu status based on the current state of the UVLED pin
    /// @attention this should be run as often as possible
    void UpdateMainMenuStatus()
    {
        if(digitalRead(UVLED) == HIGH)
        {
            _mainMenu.SetBannerText("UV LED ON!");
        }
        else
        {
            _mainMenu.SetBannerText("UV LED OFF");
        }
    }

    /// @brief Plays a tone to the user indicating that curing has completed
    void PlayCureCompleteBeep()
    {
        static int numberOfBeeps = 0;
        // If we have beeped less than 3 times
        if(numberOfBeeps < CURE_COMPLETE_BEEP_NUMBER)
        {
            // Increment number of beeps, Play tone, and start the timer again
            numberOfBeeps++;
            _toneGenerator.playTone(CURE_COMPLETE_BEEP_FREQ, CURE_COMPLETE_BEEP_DURATION);
            _cureCompleteBeepTimer.Start();
        }
        else
        {
            // Reset the timer
            numberOfBeeps = 0;
        }
    }

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
    SysEvtQueue* _evtQ;


    /* Callbacks functions for each page and menu button */

    /// @brief Return wrapper for the built in ReturnToPreviousScreen method
    UiCallbackWrapper<UvCuringOvenUi> _returnWrapper;

    /// @brief Callback function for the UV On button in the main menu
    void UvOnAction()
    {
        _evtQ->Add(SysEvt_StartCuringPressed);
    }
   
    /// @brief Callback function for the UV Off button in the main menu
    void UvOffAction()
    {
        _evtQ->Add(SysEvt_CancelCuringPressed);
    }

    /// @brief Callback function for the Set Time button in the main menu
    void SetTimeAction()
    {
        _reusableNumericInput.Set(&_u8g2, "Exposure Time", &_selectedSettings->CuringTime, 0, 100, _returnWrapper.Execute, &_returnWrapper);
        SetScreen("numericInput");
    }
    
    /// @brief Callback function for the UV Brightness button in the main menu
    void UvBrightnessAction()
    {
        _reusableNumericInput.Set(&_u8g2, "UV Brightness", &_selectedSettings->brightness, 0, 100, _returnWrapper.Execute, &_returnWrapper);
        SetScreen("numericInput");
    }

    /// @brief Callback function for the Motor Speed button in the main menu
    void MotorSpeedAction()
    {
        _reusableNumericInput.Set(&_u8g2, "Motor Speed", &_selectedSettings->MotorSpeed, 0, 100, _returnWrapper.Execute, &_returnWrapper);
        SetScreen("numericInput");
    }

    /// @brief Callback function for the useless button in the main menu
    void UselessAction()
    {
        
    }

};

void SetScreenRefreshFlag();

/// @brief A timer to set the refresh screen flag 
static Timer _screenRefreshTimer(SCREEN_REFRESH_RATE, SetScreenRefreshFlag);
/// @brief Refresh screen flag. When true the screen should be reset
volatile bool _refreshScreen = true;

static UvCuringOvenUi* uiPtr;

Semaphore gUiCompletedSem;

/// @brief Sets the refresh flag to trigger this UI implementation to update when UiUpdate is run
void SetScreenRefreshFlag()
{
    _refreshScreen = true;
    _screenRefreshTimer.Start();
}

void SetupUi(void* param)
{
    SelectedSettings* settings = static_cast<SelectedSettings*>(param);

    Serial.println("Initialising UI");

    static UvCuringOvenUi uiObj(KNOB_ENC_1, KNOB_ENC_2, KNOB_PUSH, U8G2_R0, /* cs=*/ 10, /* dc=*/ 8, /* reset=*/ 7, SPEAKER_PIN, settings, &gEventQ);
    uiPtr = &uiObj;
    uiPtr->SetupMainMenu();
    uiPtr->SetupNumericInput();
    _screenRefreshTimer.Start();
}

void RunUi(void* param)
{
    // Check for and react to any user inputs
    uiPtr->CheckUserInput();

    uiPtr->UpdateMainMenuStatus();

    // If the refresh flag is set, refresh the screen.
    if(_refreshScreen)
    {
        _refreshScreen = false;
        uiPtr->Draw();
    }


    if(gUiCompletedSem.IsSignaled())
    {
        uiPtr->PlayCureCompleteBeep();
    }
}

