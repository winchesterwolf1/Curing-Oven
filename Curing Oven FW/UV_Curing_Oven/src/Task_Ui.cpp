#include "UiTask.h"
#include "Ui.h"
#include "Task_EventManager.h"
#include "Timer.h"
#include "UvCuringOvenUi.h"

class UiTask : public Task
{
    public:

    UiTask() :
        Task(1),
        _screenRefreshTimer(SCREEN_REFRESH_RATE, ScreenRefreshCallbackWrapper, this)
    {
        
    }

    /// @brief A timer to set the refresh screen flag 
    Timer _screenRefreshTimer;
    /// @brief Refresh screen flag. When true the screen should be reset
    volatile bool _refreshScreen = true;

    UvCuringOvenUi* uiPtr;


    /// @brief Sets the refresh flag to trigger this UI implementation to update when UiUpdate is run
    void SetScreenRefreshFlag()
    {
        _refreshScreen = true;
        _screenRefreshTimer.Start();
    }

    static void ScreenRefreshCallbackWrapper(void* obj)
    {
        UiTask* task = static_cast<UiTask*>(obj);
        task->SetScreenRefreshFlag();
    }

    void Setup(void* param)
    {
        SelectedSettings* settings = static_cast<SelectedSettings*>(param);

        Serial.println("Initialising UI");

        static UvCuringOvenUi uiObj(
            KNOB_ENC_1, KNOB_ENC_2, KNOB_PUSH, U8G2_R0, 
            /* cs=*/ 10, /* dc=*/ 8, /* reset=*/ 7, 
            SPEAKER_PIN, settings, _pRtos->Queues[Queues_SysEvt]);
        uiPtr = &uiObj;
        uiPtr->SetupMainMenu();
        uiPtr->SetupNumericInput();
        _screenRefreshTimer.Start();
    }

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
};

UiTask uiTaskObj;
Task* gUiTaskPtr = &uiTaskObj;

