#include "Mutex.h"

Mutex::Mutex() :
    _mutex(true)
{

}

bool Mutex::Lock()
{
    if(_mutex)
    {
        _mutex = false;
        return true;
    }
    else
    {
        return false;
    }
}

void Mutex::Release()
{
    _mutex = true;
}
