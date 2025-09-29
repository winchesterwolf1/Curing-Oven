#include "Timer.h"
#ifndef TIMESERVER_H // include guard
#define TIMESERVER_H

/// @brief Adds timer to the linked list and sets its completion timestamp.
/// @param timer 
void TimeServerAddToList(Timer* timer);

/// @brief Removes timer from the timer list.
/// @param timer 
void TimeServerRemoveFromList(Timer* timer);

/// @brief Initialises the timers for the time server
void TimeServerInit();

#endif /* TIMESERVER_H */