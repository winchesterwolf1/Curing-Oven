#ifndef PSEUDORTOS_H
#define PSEUDORTOS_H

#include "Task.h"
#include "Queue.h"
#include "Semaphore.h"
#include "Mutex.h"
#include "CountingSemaphore.h"

#define SEMAPHORE_ARRAY_LEN 20
#define QUEUE_ARRAY_LEN 5
#define MUTEX_ARRAY_LEN 5
#define COUNTSEMAPHORE_ARRAY_LEN 5

class PseudoRtos {
    public:

        PseudoRtos(void* appEnvInfo);

        void RegisterTask(Task* task);
        void SuspendTask(Task* task);
        void ResumeTask(Task* task);

        void RegisterQueue(QueueBase* queuePtr, unsigned int index);
        void RegisterSemaphore(Semaphore* semPtr, unsigned int index);
        void RegisterMutex(Mutex* mutxPtr, unsigned int index);
        void RegisterCountingSemaphore(CountingSemaphore* cntSemPtr, unsigned int index);

        void Init();
        void Run();
        void RunThrough();

        Semaphore* Semaphores[SEMAPHORE_ARRAY_LEN] = {nullptr};
        QueueBase* Queues[QUEUE_ARRAY_LEN] = {nullptr};
        Mutex* Mutexes[MUTEX_ARRAY_LEN] = {nullptr};
        CountingSemaphore* CountSemapores[COUNTSEMAPHORE_ARRAY_LEN] = {nullptr};

    private:
        void* _appEnvInfo;
        Task* _headTask;
};

#endif // PSEUDORTOS_H