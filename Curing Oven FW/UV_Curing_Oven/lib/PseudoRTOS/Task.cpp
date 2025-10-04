#include "Task.h"

Task::Task(TaskFunction_t setupFunc, TaskFunction_t taskFunc, void* parameter, unsigned int priority) :
    _setupFunc(setupFunc),
    _taskFunc(taskFunc),
    _parameter(parameter),
    _priority(priority),
    _state(TaskState::Ready),
    _nextTask(nullptr)
{

}

Task::~Task()
{

}

void Task::Setup()
{
    _setupFunc(_parameter);
}

void Task::RunTask()
{
    _taskFunc(_parameter);
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


