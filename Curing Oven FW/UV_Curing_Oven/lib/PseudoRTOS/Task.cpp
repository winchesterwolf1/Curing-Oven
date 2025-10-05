#include "Task.h"

Task::Task(unsigned int priority) :
    _priority(priority),
    _state(TaskState::Ready),
    _nextTask(nullptr)
{

}

Task::TaskState Task::GetState() const
{
    return _state;
}

void Task::SetState(const TaskState sate)
{
    _state = sate;
}

Task* Task::GetNextTask() const
{
    return _nextTask;
}

void Task::SetNextTask(Task* taskPtr)
{
    _nextTask = taskPtr;
}

unsigned int Task::GetPriority() const
{
    return _priority;
}

void Task::RegisterOwner(PseudoRtos* pRtos)
{
    _pRtos = pRtos;
}

