#ifndef MOTOR_H
#define MOTOR_H

class Motor {
public:
    Motor(int pin);

    ~Motor();

    void Setup();

    void start();
    void stop();
    void setSpeed(unsigned int speed);
    int getSpeed();

private:
    int _speed;
    int _pwmPin;
};

#endif // MOTOR_H