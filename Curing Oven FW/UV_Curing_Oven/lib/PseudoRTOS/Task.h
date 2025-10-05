#ifndef PSEUDORTOS_TASK_H
#define PSEUDORTOS_TASK_H

class PseudoRtos;

// Task function pointer type
typedef void (*TaskFunction_t)(void*);

// Task base class definition
class Task {
    public:

        // Task state enumeration
        enum class TaskState{
            Ready = 0,
            Running,
            Suspended
        };

        Task(unsigned int priority);

        virtual void Setup(void* param) = 0;

        virtual void RunTask(void* param) = 0;

        TaskState GetState() const;

        void SetState(const TaskState sate);

        Task* GetNextTask() const;

        void SetNextTask(Task* taskPtr);

        unsigned int GetPriority() const;

        void RegisterOwner(PseudoRtos* pRtos);
    
    protected:
        PseudoRtos* _pRtos;

    private:
        unsigned int _priority;
        TaskState _state;
        Task* _nextTask;
};

#endif // PSEUDORTOS_TASK_H
