#include "Semaphore.h"

Semaphore::Semaphore() :
    _sem(false)
{
}

bool Semaphore::IsSignaled()
{
    bool sem = _sem;
    _sem = false;
    return sem;
}

void Semaphore::Signal()
{
    _sem = true;
}