#include "CountingSemaphore.h"

CountingSemaphore::CountingSemaphore(unsigned int initialCount, unsigned int maxValue) :
    _maxValue(initialCount),
    _count(initialCount)
{

}

bool CountingSemaphore::Give()
{
    if(_count == _maxValue)
    {
        return false;
    }
    _count++;
    return true;
}

bool CountingSemaphore::Take()
{
    if(_count == 0)
    {
        return false;
    }
    _count--;
    return true;
}
