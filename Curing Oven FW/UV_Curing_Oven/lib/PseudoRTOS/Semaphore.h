#ifndef PSEUDORTOS_SEMAPHORE_H
#define PSEUDORTOS_SEMAPHORE_H

class Semaphore {
public:
    Semaphore();

    bool IsSignaled();
    void Signal();

private:
    bool _sem;
};

#endif // PSEUDORTOS_SEMAPHORE_H