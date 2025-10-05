#ifndef MOTOR_H
#define MOTOR_H

/// @brief Class for controlling the motor for the rotating platform
class Motor {
public:
    /// @brief Save default values of this class
    /// @param pin PWM pin to use to drive this motor
    Motor(int pin);

    /// @brief Destructor
    ~Motor();

    /// @brief Initialise pin registers and set output to default state
    void Setup();

    /// @brief Start the PWM output of the motor at the current speed
    void start();
    /// @brief Stop PWM output and set output to low
    void stop();
    
    /// @brief Set the speed which the motor will rotate at when Start() is called
    /// @param speed Percentage value from 0 - 100
    void setSpeed(unsigned int speed);

    /// @brief Get the current speed setting of the motor
    /// @return current speed setting as a percentage
    int getSpeed();

private:
    /// @brief Current speed setting. Value from 0 - 255
    int _speed;
    /// @brief Output pin
    int _pwmPin;
};

#endif // MOTOR_H