#include "Mutex.h"

/// @brief Initialise mutex to be available to take
Mutex::Mutex() :
    _mutex(true)
{

}

/// @brief check mutex state. if mutex is already locked return false
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

/// @brief simply set mutex to true
void Mutex::Release()
{
    _mutex = true;
}
