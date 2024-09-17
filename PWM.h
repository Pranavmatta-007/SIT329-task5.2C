#ifndef PWM_H
#define PWM_H

#include <Arduino.h>

class PWM {
public:
    PWM();
    bool set_PWM(int frequency, int dutyCycle);
    bool output_PWM(int port, int pin_number);
    void start_PWM();
    void stop_PWM();

private:
    void configureTCC(int frequency, int dutyCycle);
};

#endif
