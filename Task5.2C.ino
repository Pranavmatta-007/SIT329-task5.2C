#include "Arduino.h"
#include "wiring_private.h"  // Required for pinPeripheral

#define LED_PIN 2
#define TCC0_CLK_ID 0  // GCLK ID for TCC0

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);

    // Enable TCC0 clock source
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_GEN_GCLK0 |
                        GCLK_CLKCTRL_ID_TCC0_TCC1;
    while (GCLK->STATUS.bit.SYNCBUSY);

    // Configure TCC0
    TCC0->CTRLA.reg &= ~TCC_CTRLA_ENABLE;  // Disable TCC0 before configuration
    while (TCC0->SYNCBUSY.bit.ENABLE);

    TCC0->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM;  // Normal PWM
    while (TCC0->SYNCBUSY.bit.WAVE);

    TCC0->PER.reg = 1499;  // Period for PWM frequency
    while (TCC0->SYNCBUSY.bit.PER);

    TCC0->CC[0].reg = 0;  // Initial duty cycle (0%)
    while (TCC0->SYNCBUSY.bit.CC0);

    pinPeripheral(LED_PIN, PIO_TIMER);  // Set pin for PWM function

    TCC0->CTRLA.reg |= TCC_CTRLA_ENABLE;  // Enable TCC0
    while (TCC0->SYNCBUSY.bit.ENABLE);

    Serial.println("TCC0 configured for PWM.");
}

void loop() {
    for (int dutyCycle = 0; dutyCycle <= 100; dutyCycle++) {
        int compareValue = (1499 * dutyCycle) / 100;
        TCC0->CC[0].reg = compareValue;
        while (TCC0->SYNCBUSY.bit.CC0);
        delay(10);  // Smooth transition
    }

    for (int dutyCycle = 100; dutyCycle >= 0; dutyCycle--) {
        int compareValue = (1499 * dutyCycle) / 100;
        TCC0->CC[0].reg = compareValue;
        while (TCC0->SYNCBUSY.bit.CC0);
        delay(10);  // Smooth transition
    }
}
