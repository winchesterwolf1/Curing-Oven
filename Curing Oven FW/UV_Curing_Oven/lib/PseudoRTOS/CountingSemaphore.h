#ifndef COUNTINGSEMAPHORE_H
#define COUNTINGSEMAPHORE_H

/// @brief 
class CountingSemaphore {
public:
    CountingSemaphore(unsigned int initialCount, unsigned int maxValue);

    bool Give();

    bool Take();

private:
    unsigned int const _maxValue;
    unsigned int _count;
};

#endif // COUNTINGSEMAPHORE_H