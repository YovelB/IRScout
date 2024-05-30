#pragma once

#include <Arduino.h>

namespace TGC {
    class Transmitter {
    public:
        explicit Transmitter(int signal = 11, double desiredFreq = 300, int dutyCycle = 50);

        /**
         * @brief Set up the IR output.
         * This method sets up the timers and start the IR output.
         * This method should be called in the setup function.
         */
        void setup();
        /**
         * @brief Handle the interrupts.
         * This function handles the interrupt by toggling the OC2A on Compare Match and ensuring the signal is off.
         * This method should be called from an ISR.
         */
        void handleInterrupt() const;
        bool isReady() const { return isSetupDone; }
        enum TimerMode {
            CTC,
            FAST_PWM
        };

    private:
        const int signal{};
        const double desiredFreq{};
        const int dutyCycle{};
        bool isSetupDone{false};

        void setUpTimer1() const;
        static void setUpTimer2();

        /**
         * @brief Calculate the timer frequency.
         * This function calculates the timer frequency based on the frequency, prescaler, and timer mode.
         * @param freq: The frequency.
         * @param prescaler: The prescaler.
         * @param timerMode: The timer mode (CTC or FAST_PWM).
         */
        static unsigned long calcTimerFreq(double freq, int prescaler, TimerMode timerMode);
        /**
         * @brief Calculate the duty cycle.
         * This function calculates the duty cycle based on the OCRXA value.
         * @param dutyCycle: The duty cycle.
         * @param OCRXA: The OCRXA value.
         * @return The calculated duty cycle.
         */
        static unsigned long calcTimerDutyCycle(int dutyCycle, int OCRXA);
    };
} // namespace TGC