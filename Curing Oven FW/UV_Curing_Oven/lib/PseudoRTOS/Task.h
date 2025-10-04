#ifndef PSEUDORTOS_TASK_H
#define PSEUDORTOS_TASK_H

// Task function pointer type
typedef void (*TaskFunction_t)(void*);

// Task class definition
class Task {
    public:

        // Task state enumeration
        enum class TaskState{
            Ready = 0,
            Running,
            Suspended
        };

        Task(TaskFunction_t setupFunc, TaskFunction_t taskFunc, void* parameter, unsigned int priority);
        ~Task();

        void Setup();

        void RunTask();

        TaskState GetState() const;

        void SetState(const TaskState sate);

        Task* GetNextTask() const;

        void SetNextTask(Task* taskPtr);

        unsigned int GetPriority() const;

    private:
    
        TaskFunction_t _setupFunc;
        TaskFunction_t _taskFunc;
        void* _parameter;
        unsigned int _priority;
        TaskState _state;
        Task* _nextTask;
};

#endif // PSEUDORTOS_TASK_H
