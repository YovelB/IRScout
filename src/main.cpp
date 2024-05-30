#include <Arduino.h>

#include "Transmitter.hpp"

constexpr int LED = 11;                   // Timer2 output pin.
constexpr int MODULATED_FREQUENCY = 150;  // 150 Or 300 in Hz.
constexpr int DUTY_CYCLE = 30;            // Or 50 in percentage.
TGC::Transmitter controller(LED, MODULATED_FREQUENCY, DUTY_CYCLE);

// TODO I should check to remove one of the interrupt functions.
// TODO and move these to the class.
/**
 * @brief Timer 1 Compare Match B interrupt.
 * This function handles the interrupt by calling the handleInterrupt function of the IRController instance.
 * This function is called when the Timer 1 Compare Match B interrupt is triggered.
 * The same for Timer 1 Compare Match A interrupt.
 */
ISR(TIMER1_COMPB_vect) {
    if (controller.isReady()) {
        controller.handleInterrupt();
    }
}

ISR(TIMER1_COMPA_vect) {
    if (controller.isReady()) {
        controller.handleInterrupt();
    }
}

void setup() {
    controller.setup();
}

void loop() {
}