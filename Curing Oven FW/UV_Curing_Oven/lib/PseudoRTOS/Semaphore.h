#ifndef PSEUDORTOS_SEMAPHORE_H
#define PSEUDORTOS_SEMAPHORE_H

/// @brief Semaphore implemenation for the PseudoRTOS library
class Semaphore {
public:
    /// @brief Constructor for initialising default values
    Semaphore();

    /// @brief Check if this semaphore has been signaled
    /// @return true if it was signaled since last checked
    bool IsSignaled();

    /// @brief Signal this semaphore
    void Signal();

private:
    /// @brief Internal semaphore state
    bool _sem;
};

#endif // PSEUDORTOS_SEMAPHORE_H