#include "Timer.h"
#include "TimeServer.h"

/// @brief Private method to use if timeout timer is initialised.
void DoNothing(){}

/// @brief simply save the input parameters, and set the non-object callback function to doNothing
Timer::Timer(unsigned int timerLength, TimerCallbackAction objectCallbackFunction, void* objectCallbackContext)
{
    _timerLength = timerLength;
    _objectCallbackFunction = objectCallbackFunction;
    _objectCallbackContext = objectCallbackContext;
    _callbackFunction = DoNothing;
}


/// @brief Simply save the input parameters
Timer::Timer(unsigned int timerLength, void (callbackFunction)())
{
    _timerLength = timerLength;
    _callbackFunction = callbackFunction;
}

/// @brief Simply save the input parameters
Timer::Timer(unsigned int timerLength)
{
    _timerLength = timerLength;
    _callbackFunction = DoNothing;
}

/// @brief Run the callback function and set timer frinished to true on completion
void Timer::RunCallback()
{
    // Set Completion flag
    _timerFinished = true;

    // Run the object callback function if initialised, 
    // otherwise run the static callback function
    if(_objectCallbackFunction != nullptr &&
        _objectCallbackContext != nullptr)
    {
        _objectCallbackFunction(_objectCallbackContext);
    }
    else
    {
        _callbackFunction();
    }
}

/// @brief Start the timer by adding it to the time server and set finished to false.
/// If timer is already running, nothing will happen
void Timer::Start()
{
    if(_timerFinished == false)
    {
        return;
    }
    TimeServerAddToList(this);
    _timerFinished = false;
}

/// @brief Remove timer from the time server and set finished to true
void Timer::Stop()
{
    TimeServerRemoveFromList(this);
    _timerFinished = true;
}

/// @brief remove and re-add to the time server. this way linked list does not break.
void Timer::Restart()
{
    TimeServerRemoveFromList(this);
    TimeServerAddToList(this);
    _timerFinished = false;
}

/// @brief return the timer finished flag
bool Timer::IsCompleted()
{
    return _timerFinished;
}

/// @brief set the timer length
void Timer::SetTimerLength(unsigned int length)
{
    _timerLength = length;
}





