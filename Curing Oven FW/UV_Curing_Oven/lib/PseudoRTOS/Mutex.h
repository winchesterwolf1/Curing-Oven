#ifndef PSEUDORTOS_MUTEX_H
#define PSEUDORTOS_MUTEX_H

/// @brief Mutex implementation in the PseudoRTOS library
class Mutex {
public:
    /// @brief Constructor to set default state
    Mutex();

    /// @brief Lock this mutex
    /// @return false if mutex is already locked
    bool Lock();

    /// @brief Release hold of this mutex
    void Release();

private:
    /// @brief Saved mutex state
    bool _mutex;
};

#endif // PSEUDORTOS_MUTEX_H