#include "IRReceiver.hpp"

namespace GC {
    IRReceiver::IRReceiver(int pin) : pin(pin) {}

    void IRReceiver::setup() const { pinMode(pin, INPUT); }

    bool IRReceiver::isCorrectIRSignal(double desiredFreq, double desiredDutyCycle) {
        this->measure();
        return isFreqInBounds(desiredFreq) && isDutyCycleInBounds(desiredDutyCycle);
    }

/**
     * @brief Measures the frequency and duty cycle of the IR signal.
     *
     * This function uses the pulseIn function to measure the time the signal is high and low.
     * The period is calculated by adding the time the signal is high and low.
     * The frequency is calculated by taking the reciprocal of the period.
     * The duty cycle is calculated by dividing the time the signal is high by the period.
     */
    void IRReceiver::measure() {
        this->tOn = pulseIn(pin, HIGH);
        this->tOff = pulseIn(pin, LOW);
        this->period = tOn + tOff;
        double periodInSec = static_cast<double>(period) / FROM_MICRO_SEC_TO_SEC;
        this->freq = 1.0 / periodInSec;
        this->dutyCycle = (static_cast<float>(tOn) / static_cast<float>(period)) * VALUE_TO_PERCENT;
    }

    void IRReceiver::print(unsigned long delayInMs) const {
        const String output =
                "Frequency: " + String(freq) + " Hz, Duty Cycle: " + String(dutyCycle) + "%\n"
                + "Period: " + String(period) + " ms\n"
                + "tOn: " + String(tOn) + " ms, tOff: " + String(tOff) + " ms\n";
        Serial.print(output);
        delay(delayInMs);
    }

    bool IRReceiver::isFreqInBounds(double desiredFreq, double tolerance) const {
        const double lowerBound = desiredFreq * (ONE - tolerance / VALUE_TO_PERCENT);
        const double upperBound = desiredFreq * (ONE + tolerance / VALUE_TO_PERCENT);
        return freq >= lowerBound && freq <= upperBound;
    }

    bool IRReceiver::isDutyCycleInBounds(double desiredDutyCycle, double tolerance) const {
        const double lowerBound = desiredDutyCycle * (ONE - tolerance / VALUE_TO_PERCENT);
        const double upperBound = desiredDutyCycle * (ONE + tolerance / VALUE_TO_PERCENT);
        return dutyCycle >= lowerBound && dutyCycle <= upperBound;
    }
}