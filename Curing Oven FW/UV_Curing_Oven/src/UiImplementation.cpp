#include "UiImplementation.h"
#include "Timer.h"

#define MAIN_MENU_NUM_ITEMS 6

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
        SelectedSettings* selectedSettings, void(**systemFunctions)())
        :
        Ui(knobS1, knobS2, knobKey, oledRot, oledCs, oledDc, oledReset),
        _selectedSettings(selectedSettings),
        _systemFunctions(systemFunctions),
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
        _reusableNumericInput.Set(&_u8g2, "Exposure Time", &_selectedSettings->on_time, 0, 100, _returnWrapper.Execute, &_returnWrapper);
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

    private:

    /* Screen Objects */

    /// @brief Main Menu screen
    Menu _mainMenu;
    /// @brief Re-usable numeric input screen 
    NumericInput _reusableNumericInput;


    /* Settings objects and system functions to be manipulated by callbacks and pages */

    /// @brief Setting object list to modify in the menu
    SelectedSettings* _selectedSettings;
    /// @brief System functions to be run by menu buttons
    void(**_systemFunctions)();


    /* Callbacks functions for each page and menu button */

    /// @brief Return wrapper for the built in ReturnToPreviousScreen method
    UiCallbackWrapper<UvCuringOvenUi> _returnWrapper;

    /// @brief Callback function for the UV On button in the main menu
    void UvOnAction()
    {
        _systemFunctions[SysFuncLedStart]();
    }
   
    /// @brief Callback function for the UV Off button in the main menu
    void UvOffAction()
    {
        _systemFunctions[SysFuncLedStop]();
    }

    /// @brief Callback function for the Set Time button in the main menu
    void SetTimeAction()
    {
        _reusableNumericInput.Set(&_u8g2, "Exposure Time", &_selectedSettings->on_time, 0, 100, _returnWrapper.Execute, &_returnWrapper);
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
        _reusableNumericInput.Set(&_u8g2, "Motor Speed", &_selectedSettings->motor_speed, 0, 100, _returnWrapper.Execute, &_returnWrapper);
        SetScreen("numericInput");
    }

    /// @brief Callback function for the useless button in the main menu
    void UselessAction()
    {
        
    }

};

void SetScreenRefreshFlag();

static Timer _screenRefreshTimer(SCREEN_REFRESH_RATE, SetScreenRefreshFlag);
bool _refreshScreen = true;

void SetScreenRefreshFlag()
{
    _refreshScreen = true;
    _screenRefreshTimer.Start();
}

/// @brief Crete Ui objects and initialise Ui to start
/// @param selectedSettings 
/// @param systemFunctions 
/// @return 
Ui* CreateUi(SelectedSettings* selectedSettings, void(**systemFunctions)())
{
    static UvCuringOvenUi UiObj(KNOB_ENC_1, KNOB_ENC_2, KNOB_PUSH, U8G2_R0, /* cs=*/ 10, /* dc=*/ 8, /* reset=*/ 7, selectedSettings, systemFunctions);
    UiObj.SetupMainMenu();
    UiObj.SetupNumericInput();
    _screenRefreshTimer.Start();
    return &UiObj;
}

void UiUpdate(Ui* uiObj)
{
    uiObj->CheckUserInput();

    UvCuringOvenUi* castUiObj = static_cast<UvCuringOvenUi*>(uiObj);
    castUiObj->UpdateMainMenuStatus();

    if(_refreshScreen)
    {
        _refreshScreen = false;
        uiObj->Draw();
    }
}




