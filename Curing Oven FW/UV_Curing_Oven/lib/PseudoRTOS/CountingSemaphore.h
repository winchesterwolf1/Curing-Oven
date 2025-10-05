#ifndef PSEUDORTOS_COUNTINGSEMAPHORE_H
#define PSEUDORTOS_COUNTINGSEMAPHORE_H

/// @brief Counting Semaphore implementation for this PseudoRTOS library
class CountingSemaphore {
public:
    /// @brief Constructor for the counting semaphore
    /// @param initialCount Sets the initial value for the counter
    /// @param maxValue Sets the maximum number of semaphores that are able to be given
    CountingSemaphore(unsigned int initialCount, unsigned int maxValue);

    /// @brief Give a semaphore to the counting semaphore
    /// @return false if counting semaphore is full
    bool Give();

    /// @brief Take a semaphore from the counting semaphore
    /// @return false if there are none available 
    bool Take();

private:
    /// @brief Max value to count to
    unsigned int const _maxValue;
    /// @brief Current count
    unsigned int _count;
};

#endif // PSEUDORTOS_COUNTINGSEMAPHORE_H