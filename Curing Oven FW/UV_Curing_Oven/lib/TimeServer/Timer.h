#ifndef TIMER_H // include guard
#define TIMER_H

typedef unsigned long timestamp_t;

// A generic C-style callback function pointer that takes a void* context
typedef void (*TimerCallbackAction)(void* object);


/// @brief Timer class to allow for event driven timer functions.
/// @attention Objects must be declared static else firmware will crash.
/// @attention TimeServer must be initialised before calling the Start() method.
class Timer
{
    private:
        /// @brief Time of completion for this timer
        timestamp_t _completionTimeStamp;

        /// @brief address of next timer in the linked list
        Timer* _nextTimer = nullptr;

        /// @brief Callback function to call on timer completion when associated to an object
        TimerCallbackAction _objectCallbackFunction = nullptr;

        /// @brief Context for the callback function when associated to an object
        void* _objectCallbackContext = nullptr;

        /// @brief Function pointer to be called on test completion
        /// @attention Callback function must not take more than 1ms to execute
        void (*_callbackFunction)();

        /// @brief the length of the timer in ms
        unsigned int _timerLength;

        /// @brief Indication of if the timer has been stopped or completed its timeout.
        /// Initialise true, as we have not started the timer yet.
        bool _timerFinished = true;
        
        /// @brief Runs the callback set during initialisation
        void RunCallback();

        // Allow specific functions in the time server to access these private properties
        friend void TimeServerAddToList(Timer* timer);
        friend void TimeServerRemoveFromList(Timer* timer);
        friend void ServiceTimeServer();

    public:
        /// @brief Constructor for a object callback timer
        /// @param timerLength length of the timer
        /// @param objectCallbackFunction static callback function from within the target object
        /// @param objectCallbackContext target object context
        Timer(unsigned int timerLength, TimerCallbackAction objectCallbackFunction, void* objectCallbackContext);

        /// @brief Constructor for a static callback timer
        /// @param timerLength length of timer
        /// @param callbackFunction Callback function to run on timer completion. Note must take less than 1ms to complete, and be declared static.
        Timer(unsigned int timerLength, void (*callbackFunction)());

        /// @brief Constructor for a timeout timer
        /// @param timerLength length of timer
        Timer(unsigned int timerLength);

        /// @brief Starts the timer
        /// @param timer Timer to start
        void Start();

        /// @brief Stops the timer
        /// @param timer Timer to Stop
        void Stop();

        /// @brief Restarts the timer
        /// @param timer Timer to re-start
        void Restart();

        /// @brief Checks if the timer has completed
        /// @param timer Timer to check
        /// @return True if timer is no longer running
        bool IsCompleted();

        /// @brief Extension method to allow editing of timer length after initialisation
        /// @param length New length of timer
        void SetTimerLength(unsigned int length);
};

#endif /* TIMER_H */