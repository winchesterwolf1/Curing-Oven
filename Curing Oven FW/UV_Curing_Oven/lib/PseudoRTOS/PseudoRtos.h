#ifndef PSEUDORTOS_H
#define PSEUDORTOS_H

#include "Task.h"
#include "Queue.h"
#include "Semaphore.h"
#include "Mutex.h"
#include "CountingSemaphore.h"

// Define PRTOS memory macros if not defined elsewhere
#ifndef SEMAPHORE_ARRAY_LEN
    // Maximum of items to store in the semaphore array
    #define SEMAPHORE_ARRAY_LEN 20
#endif
#ifndef QUEUE_ARRAY_LEN
    // Maximum of items to store in the queue array
    #define QUEUE_ARRAY_LEN 5
#endif
#ifndef MUTEX_ARRAY_LEN
    // Maximum of items to store in the mutex array
    #define MUTEX_ARRAY_LEN 5
#endif
#ifndef COUNTSEMAPHORE_ARRAY_LEN
    // Maximum of items to store in the counting semaphore array
    #define COUNTSEMAPHORE_ARRAY_LEN 5
#endif

/// @brief PseudoRTOS main class used to run the RTOS
class PseudoRtos {
    public:
        /// @brief Constructor to save default values
        /// @param appEnvInfo pointer to the application environment info that is passed into all tasks when run.
        PseudoRtos(void* appEnvInfo);

        /// @brief Register a task to be scheduled and run by this pRTOS
        /// @param task pointer to the task
        void RegisterTask(Task* task);
        /// @brief Suspend a task to stop it from being run. Task will still be setup as normal
        /// @param task pointer to the task to suspend
        void SuspendTask(Task* task);
        /// @brief Resume a task that was previously suspended.
        /// @param task pointer to the task to resume
        void ResumeTask(Task* task);

        /// @brief Register a queue to be handled by the RTOS
        /// @param queuePtr pointer to the queue to add
        /// @param index Queue array index to save the queue to
        void RegisterQueue(QueueBase* queuePtr, unsigned int index);
        /// @brief Register a semaphore to be handled by the RTOS
        /// @param semPtr pointer to the semaphore to add
        /// @param index Semaphore array index to save the semaphore to
        void RegisterSemaphore(Semaphore* semPtr, unsigned int index);
        /// @brief Register a mutex to be handled by the RTOS
        /// @param mutxPtr pointer to the mutex to add
        /// @param index Mutex array index to save the mutex to
        void RegisterMutex(Mutex* mutxPtr, unsigned int index);
        /// @brief Register a counting semaphore to be handled by the RTOS
        /// @param cntSemPtr pointer to the counting semaphore to add
        /// @param index Counting semaphore array index to save the counting semaphore to
        void RegisterCountingSemaphore(CountingSemaphore* cntSemPtr, unsigned int index);

        /// @brief initialise all registered tasks
        void Init();

        /// @brief Take over control of the processor thread and run all tasks
        /// @attention This method should never exit
        void Run();
        
        /// @brief Run each task and return once the end of the list is reached
        void RunThrough();

        /// @brief Semaphore array accessable by all tasks
        Semaphore* Semaphores[SEMAPHORE_ARRAY_LEN] = {nullptr};
        /// @brief Queue array accessable by all tasks
        QueueBase* Queues[QUEUE_ARRAY_LEN] = {nullptr};
        /// @brief Mutex array accessable by all tasks
        Mutex* Mutexes[MUTEX_ARRAY_LEN] = {nullptr};
        /// @brief Counting Semaphore array accessable by all tasks
        CountingSemaphore* CountSemapores[COUNTSEMAPHORE_ARRAY_LEN] = {nullptr};

    private:
        /// @brief Saved pointer to the application environment infomation
        void* _appEnvInfo;
        /// @brief Head task of the linked list
        Task* _headTask;
};

#endif // PSEUDORTOS_H