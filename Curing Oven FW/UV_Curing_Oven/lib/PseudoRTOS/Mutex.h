#ifndef PSEUDORTOS_MUTEX_H
#define PSEUDORTOS_MUTEX_H

class Mutex {
public:
    Mutex();
    ~Mutex();

    bool Take();

    void Release();

private:
    bool _mutex;
};

#endif // PSEUDORTOS_MUTEX_H