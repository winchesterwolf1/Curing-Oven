#include "Mutex.h"

Mutex::Mutex() :
    _mutex(true)
{

}

Mutex::~Mutex()
{

}

bool Mutex::Take()
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
