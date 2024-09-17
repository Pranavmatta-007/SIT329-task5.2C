#include "PWM.h"

PWM::PWM() {
    // Constructor can initialize TCC or other settings
}

bool PWM::set_PWM(int frequency, int dutyCycle) {
    if (frequency <= 0 || dutyCycle < 0 || dutyCycle > 100) {
        return false;  // Invalid parameters
    }
    configureTCC(frequency, dutyCycle);  // Setup PWM
    return true;
}

bool PWM::output_PWM(int port, int pin_number) {
    // Configure the pin for PWM output (e.g., using pinMode)
    pinMode(pin_number, OUTPUT);
    // Check if TCC is available for the selected pin and port
    // Configure the port as PWM output
    return true;
}

void PWM::configureTCC(int frequency, int dutyCycle) {
    // Configure the appropriate TCC register for PWM
    // Example: Use TCC0 or TCC1 depending on the pin
    uint32_t period = SystemCoreClock / frequency;
    uint32_t compareValue = (period * dutyCycle) / 100;

    // Set the period and duty cycle in TCC registers
    TCC0->PER.reg = period;  // Set the period
    TCC0->CC[0].reg = compareValue;  // Set the duty cycle (channel 0)
    while (TCC0->SYNCBUSY.bit.PER || TCC0->SYNCBUSY.bit.CC0);
    
    TCC0->CTRLA.bit.ENABLE = 1;  // Start TCC0
    while (TCC0->SYNCBUSY.bit.ENABLE);  // Wait for synchronization
}

void PWM::start_PWM() {
    // Enable the TCC for PWM output
    TCC0->CTRLA.bit.ENABLE = 1;
}

void PWM::stop_PWM() {
    // Disable the TCC to stop PWM output
    TCC0->CTRLA.bit.ENABLE = 0;
}
