#ifndef PSEUDORTOS_H
#define PSEUDORTOS_H

#include "Task.h"
#include "Queue.h"
#include "Semaphore.h"
#include "Mutex.h"
#include "CountingSemaphore.h"

class PseudoRtos {
    public:

        PseudoRtos();

        void RegisterTask(Task* task);
        void SuspendTask(Task* task);
        void ResumeTask(Task* task);

        void Init();
        void Run();
        void RunThrough();

    private:

        Task* _headTask = nullptr;
};

#endif // PSEUDORTOS_H