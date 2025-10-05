#include "PseudoRtos.h"

/// @brief Constructor to save default parameters. Set head task to null by default
PseudoRtos::PseudoRtos(void* appEnvInfo) :
    _appEnvInfo(appEnvInfo),
    _headTask(nullptr)
{

}

/// @brief add task to linked list in order of priority. Lowest number is highest priotity
void PseudoRtos::RegisterTask(Task* task)
{
    // Save the RTOS object to the task so it can access registered semaphores etc.
    task->RegisterOwner(this);

    // Find where to place the new task in the list
    // If this one is going to come before head task, or if there are no tasks in the list...
    // 0 is highest priority
    if( _headTask == nullptr ||
        task->GetPriority() < _headTask->GetPriority())
    {
        // Save head task as the next task, and put this task as the head
        task->SetNextTask(_headTask);
        _headTask = task;
    }
    else
    {
        // because we are not at the start of the list we need to search through each task to find out placement in the list
        // We are going to seach through each task to see if our priority is higher than its NEXT task or if it is the end of the list
        // We will stop searching once we have found the task we are supposed to go after
        Task* listTask = _headTask;
        while(listTask->GetNextTask() != nullptr)
        {
            if(task->GetPriority() < listTask->GetNextTask()->GetPriority())
            {
                break;
            }

            listTask = listTask->GetNextTask();
        }

        // Set this new task's next task, and
        // Set the discovered tasks next task to this new task
        task->SetNextTask(listTask->GetNextTask());
        listTask->SetNextTask(task);
    }
    // We have slotted this task in to its place in the task list and can return.
}

/// @brief change task status to suspended
void PseudoRtos::SuspendTask(Task* task)
{
    task->SetState(Task::State::Suspended);
}

/// @brief change task status to running
void PseudoRtos::ResumeTask(Task* task)
{
    task->SetState(Task::State::Running);
}

/// @brief If the index is within the queue array length save this queue to internal list
void PseudoRtos::RegisterQueue(QueueBase* queuePtr, unsigned int index)
{
    if(index >= QUEUE_ARRAY_LEN)
    {
        return;
    }

    Queues[index] = queuePtr;
}

/// @brief If the index is within the semaphore array length save this semaphore to internal list
void PseudoRtos::RegisterSemaphore(Semaphore* semPtr, unsigned int index)
{
    if(index >= SEMAPHORE_ARRAY_LEN)
    {
        return;
    }

    Semaphores[index] = semPtr;
}

/// @brief If the index is within the semaphore array length save this semaphore to internal list
void PseudoRtos::RegisterMutex(Mutex* mutxPtr, unsigned int index)
{
    if(index >= MUTEX_ARRAY_LEN)
    {
        return;
    }

    Mutexes[index] = mutxPtr;
}

/// @brief If the index is within the semaphore array length save this semaphore to internal list
void PseudoRtos::RegisterCountingSemaphore(CountingSemaphore* cntSemPtr, unsigned int index)
{
    if(index >= COUNTSEMAPHORE_ARRAY_LEN)
    {
        return;
    }

    CountSemapores[index] = cntSemPtr;
}

/// @brief run startup functions of all tasks
void PseudoRtos::Init()
{
    Task* task = _headTask;

    // Scroll through the linked list and run each tasks setup
    while(task != nullptr)
    {
        task->Setup(_appEnvInfo);
        task = task->GetNextTask();
    }
}

/// @brief run task functions of all tasks that are not suspended. Set all task states to running when run for the first time
/// @attention This function takes over full control of the processor and will never release it
void PseudoRtos::Run()
{
    Task* task = _headTask;
    
    while(true)
    {
        // Go through the entire list of tasks
        if(task != nullptr)
        {
            switch (task->GetState())
            {
                case Task::State::Ready:
                    task->SetState(Task::State::Running);
                    // Intentional Fallthrough
                case Task::State::Running:
                    task->RunTask(_appEnvInfo);
                    // Intentional Fallthrough
                case Task::State::Suspended:
                    // Intentional Fallthrough
                default:
                    task = task->GetNextTask();
                    break;
            }            
        }
        else // task == nullptr
        {
            // Once we reach the end of the list, reset back to the start
            task = _headTask;
        }
    }
    // This function should never exit
}

/// @brief run task functions of all tasks that are not suspended. Set all task states to running when run for the first time
void PseudoRtos::RunThrough()
{
    Task* task = _headTask;

    // Go through the entire list of tasks
    while(task != nullptr)
    {
        switch (task->GetState())
        {
            case Task::State::Ready:
                task->SetState(Task::State::Running);
                // Intentional Fallthrough
            case Task::State::Running:
                task->RunTask(_appEnvInfo);
                // Intentional Fallthrough
            case Task::State::Suspended:
                // Intentional Fallthrough
            default:
                task = task->GetNextTask();
                break;
        }            
    }
    // Exit the method once we have run all tasks
}
