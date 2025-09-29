#ifndef LED_H
#define LED_H

class LED {
public:
    LED(int pin);
    void on();
    void off();
    void toggle();
    bool isOn() const;

private:
    int pin_;
    bool state_;
};

#endif // LED_H