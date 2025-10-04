#ifndef COUNTINGSEMAPHORE_H
#define COUNTINGSEMAPHORE_H

class CountingSemaphore {
public:
    CountingSemaphore();
    CountingSemaphore(int initialCount = 0);

    void Add();
    void Add(int number);

    void Subtract();
    void Subtract(int number);

    int Peek() const;

    int TakeCount();

private:
    int const _resetValue;
    int _count;
};

#endif // COUNTINGSEMAPHORE_H