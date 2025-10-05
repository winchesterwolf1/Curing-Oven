#include "Task_Ui.h"
#include "Timer.h"
#include "UvCuringOvenUi.h"

/// @brief Task for runing the reacting to user inputs from the Ui
class UiTask : public Task
{
    public:
        /// @brief constructor to save default parameters and initialse refresh timer
        UiTask() :
            Task(Task::Priority::High),
            _screenRefreshTimer(SCREEN_REFRESH_RATE, ScreenRefreshCallbackWrapper, this)
        {
            
        }

        /// @brief Create Ui, Initialise menus and start the screen refresh timer
        /// @param param application environment information
        void Setup(void* param)
        {
            SelectedSettings* settings = static_cast<SelectedSettings*>(param);

            Serial.println("Initialising UI");

            static UvCuringOvenUi uiObj(
                KNOB_ENC_1, KNOB_ENC_2, KNOB_PUSH, U8G2_R0, 
                OLED_CS_PIN, OLED_DC_PIN, OLED_RESET_PIN, 
                SPEAKER_PIN, settings, _pRtos->Queues[Queues_SysEvt]);
            uiPtr = &uiObj;

            uiPtr->Setup();
            uiPtr->SetupMainMenu();
            uiPtr->SetupNumericInput();

            _screenRefreshTimer.Start();
        }

        /// @brief Update Ui, React to user input and notify user that curing is complete if commanded
        /// @param param not used
        void RunTask(void* param)
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

            if(_pRtos->Semaphores[Sem_UiNotifyCompleted]->IsSignaled())
            {
                uiPtr->PlayCureCompleteBeep();
            }
        }

    private:

        /// @brief Pointer to the Ui instance
        UvCuringOvenUi* uiPtr;
        /// @brief A timer to set the refresh screen flag 
        Timer _screenRefreshTimer;
        /// @brief Refresh screen flag. When true the screen should be reset
        volatile bool _refreshScreen = true;

        /// @brief Sets the refresh flag to trigger the UI to update and restart the timer again
        void SetScreenRefreshFlag()
        {
            _refreshScreen = true;
            _screenRefreshTimer.Start();
        }

        /// @brief Callback wrapper for the screen refresh timer
        /// @param obj this Ui task 
        static void ScreenRefreshCallbackWrapper(void* obj)
        {
            UiTask* task = static_cast<UiTask*>(obj);
            task->SetScreenRefreshFlag();
        }
};

/// @brief Instance of the Ui Task
UiTask uiTaskObj;
/// @brief Base Task pointer of Ui Task to be used globaly for pRtos initialisation
Task* gUiTaskPtr = &uiTaskObj;

