#include "Task.h"

/// @brief Initialise default values and save priority
Task::Task(Priority priority) :
    _priority(priority),
    _state(State::Ready),
    _nextTask(nullptr)
{

}

/// @brief return stored state
Task::State Task::GetState() const
{
    return _state;
}

/// @brief save new state
void Task::SetState(const State sate)
{
    _state = sate;
}

/// @brief return saved next task pointer
Task* Task::GetNextTask() const
{
    return _nextTask;
}

/// @brief change the next task pointer to the new one
void Task::SetNextTask(Task* taskPtr)
{
    _nextTask = taskPtr;
}

/// @brief return the saved priority from construction
unsigned int Task::GetPriority() const
{
    return (int)_priority;
}

/// @brief save the pointer to the parent RTOS object
void Task::RegisterOwner(PseudoRtos* pRtos)
{
    _pRtos = pRtos;
}

