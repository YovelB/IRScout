#pragma once

#include <Arduino.h>

namespace GC {
    static constexpr const double FROM_MICRO_SEC_TO_SEC = 1000000.0;
    static constexpr const int VALUE_TO_PERCENT = 100;
    static constexpr const int ONE = 1;

    class IRReceiver {
    public:
        explicit IRReceiver(int pin);
        void setup() const;

        /**
         * @brief Check if the IR signal is correct.
         * @param desiredFreq - the desired frequency in Hz.
         * @param desiredDutyCycle - the desired duty cycle in %.
         * @return true if the IR signal is correct, false otherwise.
         */
        bool isCorrectIRSignal(double desiredFreq, double desiredDutyCycle);

        void print(unsigned long delayInMs = 1000) const;
        // Getters
        double getFreq() const { return freq; }
        double getDutyCycle() const { return dutyCycle; }

        unsigned long getPeriod() const { return period; }
        unsigned long getTOn() const { return tOn; }
        unsigned long getTOff() const { return tOff; }
    private:
        int pin;
        unsigned long tOn{}, tOff{}, period{};
        double freq{}, dutyCycle{};

        void measure();
        /**
         * @brief Check if the measured frequency is within the desired frequency bounds.
         * @param desiredFreq - the desired frequency in Hz.
         * @param tolerance - the tolerance in %.
         * @return true if the measured frequency is within the desired frequency bounds, false otherwise.
         */
        bool isFreqInBounds(double desiredFreq, double tolerance = 5) const;

        /**
         * @brief Check if the measured duty cycle is within the desired duty cycle bounds.
         * @param desiredDutyCycle - the desired duty cycle in %.
         * @param tolerance - the tolerance in %.
         * @return true if the measured duty cycle is within the desired duty cycle bounds, false otherwise.
         */
        bool isDutyCycleInBounds(double desiredDutyCycle, double tolerance = 5) const;
    };
} // namespace GC