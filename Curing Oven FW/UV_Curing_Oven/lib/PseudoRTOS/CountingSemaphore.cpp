#include "CountingSemaphore.h"

/// @brief initialise values
CountingSemaphore::CountingSemaphore(unsigned int initialCount, unsigned int maxValue) :
    _maxValue(initialCount),
    _count(initialCount)
{

}

/// @brief check if the semaphore can be returned. if we are full return false, otherwise inc counter
bool CountingSemaphore::Give()
{
    if(_count == _maxValue)
    {
        return false;
    }
    _count++;
    return true;
}

/// @brief check if count is 0. return false if so, otherwise decrement count
bool CountingSemaphore::Take()
{
    if(_count == 0)
    {
        return false;
    }
    _count--;
    return true;
}
