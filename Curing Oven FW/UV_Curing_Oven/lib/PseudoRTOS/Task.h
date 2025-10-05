#ifndef PSEUDORTOS_TASK_H
#define PSEUDORTOS_TASK_H

class PseudoRtos;

/// @brief Task implementation for the PseudoRTOS Library
class Task {
    public:

        /// @brief Task state enum to save if this task is suspended.
        enum class State{
            // Indicates that the task has been created but the pRTOS is not running
            Ready = 0,
            // Indicates that the task is currently bring run by the pRTOS
            Running,
            // Indicates that this task is suspended and will not be run by pRTOS
            Suspended
        };

        /// @brief Task priority
        enum class Priority{
            Highest = 0,
            High,
            Medium,
            Low,
            Lowest
        };

        /// @brief Constructor to save default values and set task priority
        /// @param priority Priority to set this task to
        Task(Priority priority);

        /// @brief Function to run when pRTOS.Init() is run
        /// @param param Application Environment Info passed through from pRTOS
        virtual void Setup(void* param) = 0;

        /// @brief Funcion to run when pRTOS.Run() is run and this task is not suspended
        /// @param param Application Environment Info passed through from pRTOS
        virtual void RunTask(void* param) = 0;

        /// @brief Get the current state of this task
        /// @return the current task state
        State GetState() const;

        /// @brief Set the current state of this task
        /// @param sate the new state to be set
        void SetState(const State sate);

        /// @brief Get the pointer to the next task in the linked list
        /// @return the next task pointer
        Task* GetNextTask() const;

        /// @brief Set the pointer to the next task in the linked list
        /// @param taskPtr task pointer to set
        void SetNextTask(Task* taskPtr);

        /// @brief Get the priority of this task
        /// @return the task priority
        unsigned int GetPriority() const;

        /// @brief Register the pRTOS object which will be running this task
        /// @param pRtos pointer to pRTOS to save
        void RegisterOwner(PseudoRtos* pRtos);
    
    protected:
        /// @brief pRTOS pointer to allow access to all registered semaphores etc.
        PseudoRtos* _pRtos;

    private:
        /// @brief Current task priotory
        Priority _priority;
        /// @brief Current task state
        State _state;
        /// @brief pointer to the next task in the linked list
        Task* _nextTask;
};

#endif // PSEUDORTOS_TASK_H
