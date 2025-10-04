#include "CountingSemaphore.h"

CountingSemaphore::CountingSemaphore() :
    _resetValue(0),
    _count(0)
{

}
CountingSemaphore::CountingSemaphore(int initialCount) :
    _resetValue(initialCount),
    _count(initialCount)
{

}

void CountingSemaphore::Add()
{
    Add(1);
}
void CountingSemaphore::Add(int number)
{
    _count += number;
}

void CountingSemaphore::Subtract()
{
    Subtract(1);
}
void CountingSemaphore::Subtract(int number)
{
    _count -= number;
}

int CountingSemaphore::Peek() const
{
    return _count;
}

int CountingSemaphore::TakeCount()
{
    int count = _count;
    _count = _resetValue;
    return count;
}

