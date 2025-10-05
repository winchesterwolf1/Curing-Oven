#include "Semaphore.h"

/// @brief initialise flag to false 
Semaphore::Semaphore() :
    _sem(false)
{
}

/// @brief return current flag value and set flag to false
bool Semaphore::IsSignaled()
{
    bool sem = _sem;
    _sem = false;
    return sem;
}

/// @brief set flag to true
void Semaphore::Signal()
{
    _sem = true;
}