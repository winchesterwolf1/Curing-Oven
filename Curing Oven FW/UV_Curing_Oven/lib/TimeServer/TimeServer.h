#ifndef TIMESERVER_H
#define TIMESERVER_H

#include "Timer.h"

/// @brief Adds timer to the linked list and sets its completion timestamp.
/// @param timer 
void TimeServerAddToList(Timer* timer);

/// @brief Removes timer from the timer list.
/// @param timer 
void TimeServerRemoveFromList(Timer* timer);

/// @brief Initialises the timers for the time server
void TimeServerInit();

#endif /* TIMESERVER_H */