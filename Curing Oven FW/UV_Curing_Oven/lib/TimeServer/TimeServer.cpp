#include "TimeServer.h"
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Clock adjustment definitions
#define SYS_CLK_FREQ 16000000
#define TIM0_PRESC 64
#define COUNTS_PER_MS (SYS_CLK_FREQ/TIM0_PRESC)/1000
#define COUNTER_MAX 255

// Adjustment to the compare register to fix frequency of the interrupt to 1ms
#define COMP_ADJ_1MS_FREQ_FIX (COUNTER_MAX - COUNTS_PER_MS)

/// @brief Timestamp in ms since application start
static timestamp_t _timeStamp = 0;

/// @brief Head timer of the time server linked list
static Timer* _headTimer = nullptr;

/*Function Prototypes*/

/// @brief Adjusts the timer registers to ensure exactly 1ms interrupts occur
void AdjustTimeCompareFor1Ms();

/// @brief Callback function for when a timer interrupt occurrs 
void ServiceTimeServer();
    
/*ISRs*/

/// @brief Timer 0 compare A interrupt  
ISR(TIMER0_COMPA_vect)
{
    AdjustTimeCompareFor1Ms();
    ServiceTimeServer();
}

/*Private Methods*/

/// @brief Adjusts the OCR0A register to ensure next interrupt occurrs in 1ms 
void AdjustTimeCompareFor1Ms()
{
    if(OCR0A < COMP_ADJ_1MS_FREQ_FIX)
    {
        OCR0A = OCR0A + COUNTER_MAX - COMP_ADJ_1MS_FREQ_FIX;
    }
    else
    {
        OCR0A -= COMP_ADJ_1MS_FREQ_FIX;
    }
}

/// @brief Services the time server timestamp and runs the callback for the head timer
/// if it completed. Saves the next timer as the new head before calling calback to improve stability
void ServiceTimeServer()
{
    _timeStamp++;

    if(_headTimer == nullptr)
    {
        return;
    }

    if((_headTimer->_completionTimeStamp) <= _timeStamp)
    {
        #ifdef TIMESERVER_DEBUG
        Serial.print("Timer Completed: ");
        Serial.println((int)_headTimer);
        #endif

        Timer* completedTimer = _headTimer;
        _headTimer = _headTimer->_nextTimer;

        completedTimer->RunCallback();
    }
}

/*Public Methods*/

/// @brief Initilase timers 
void TimeServerInit()
{
    // Set timer compare A register
    OCR0A = COUNTS_PER_MS;
    // Allow timer compare A interrupts
    TIMSK0 |= (1<<OCIE0A);
}

/// @brief Calculates the time stamp for when this timer is due and places the timer into the 
/// linked list in order of completion time
/// @param timer pointer to timer to add to the list
void TimeServerAddToList(Timer* timer)
{
    // Calculate completion timestamp based on timer length
    timer->_completionTimeStamp = timer->_timerLength + _timeStamp;
    
    #ifdef TIMESERVER_DEBUG
    Serial.print("Timer Added: ");
    Serial.println((int)timer);
    Serial.print("Timeout: ");
    Serial.println(timer->_completionTimeStamp);
    Serial.print("Callback: ");
    Serial.println((int)timer->_callbackFunction);
    #endif

    // Find where to place the new timer in the list
    // If this one is going to come before head timer, or if there are no timers in the list...
    if( _headTimer == nullptr ||
        timer->_completionTimeStamp < _headTimer->_completionTimeStamp)
    {
        // Save head timer as the next timer, and put this timer as the head
        timer->_nextTimer = _headTimer;
        _headTimer = timer;
    }
    else
    {
        // because we are not at the start of the list we need to search through each timer to find out placement in the list
        // We are going to seach through each timer to see if our completion time is before its NEXT timer or if it is the end of the list
        // We will stop searching once we have found the timer we are supposed to go after
        Timer* listTimer = _headTimer;
        while(listTimer->_nextTimer != nullptr)
        {
            if(timer->_completionTimeStamp < listTimer->_nextTimer->_completionTimeStamp)
            {
                break;
            }

            listTimer = listTimer->_nextTimer;
        }

        // Set this new timer's next timer, and
        // Set the discovered timers next timer to this new timer
        timer->_nextTimer = listTimer->_nextTimer;
        listTimer->_nextTimer = timer;
    }
    // We have slotted this timer in to its place in the timer list and can return.
}

/// @brief Searches through the linked list for the reference to a specified timer and
/// removes it from the list by replacing the reference with the specified timers next timer.
/// @param timer 
void TimeServerRemoveFromList(Timer* timer)
{
    // If there are no timers in the list then we can simply return
    if(_headTimer == nullptr)
    {
        return;
    }

    // If this timer is the head timer, then set the head timer to the this timers next timer and reutrn
    if(_headTimer == timer)
    {
        #ifdef TIMESERVER_DEBUG
        Serial.print("Timer Removed: ");
        Serial.println((int)timer);
        #endif

        _headTimer = timer->_nextTimer;
        return;
    }

    // Search through the linked list untill you find the timer which points to this timer
    Timer* listTimer = _headTimer;
    while(listTimer->_nextTimer != nullptr)
    {
        if(listTimer->_nextTimer == timer)
        {
            break;
        }

        listTimer = listTimer->_nextTimer;
    }

    // If we ran through the list and didnt find the timer, we can simply return
    if(listTimer->_nextTimer == nullptr)
    {
        return;
    }

    #ifdef TIMESERVER_DEBUG
    Serial.print("Timer Removed: ");
    Serial.println((int)timer);
    #endif

    // Remove this timer from the list, by setting the found timer's next timer to
    // this timers next timer.
    listTimer->_nextTimer = timer->_nextTimer;
}

