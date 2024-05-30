#include "Transmitter.hpp"

namespace TGC {
    Transmitter::Transmitter(int signal, double desiredFreq, int dutyCycle)
            : signal(signal), desiredFreq(desiredFreq), dutyCycle(dutyCycle) {}

    void Transmitter::setup() {
        noInterrupts();
        pinMode(this->signal, OUTPUT);

        TGC::Transmitter::setUpTimer2();
        this->setUpTimer1();

        this->isSetupDone = true;
        interrupts();
    }

    void Transmitter::handleInterrupt() const {
        TCCR2A ^= _BV(COM2A0);  // toggle OC2A on Compare Match
        if ((TCCR2A & _BV(COM2A0)) == 0) {
            digitalWrite(this->signal, LOW);  // ensure off
        }
    }

// TODO look up about the correct documentation for this function.
/**
 * @brief Set up Timer 1.
 * Timer 1 is set up at pin 9(OC1A) and pin 10(OC1B)(Disabled, normal port).
 * Timer Mode 15 Fast PWM.
 *
 * I don't use OCR1A. Hence, it is disabled. f(OCR1A) = fclk/(2*N*(OCR1A+1)),
 * fclk = 16MHz, N-prescaler = 256.
 * I only use the interrupt of OCR1B. Hence, it is also disabled.
 * f(OCR1B) = fclk/(N*(OCR1B+1)), fclk = 16MHz, N-prescaler = 256.
 *
 * freq - for 150Hz - 16MHz / 256 / 150Hz = 416 - 1 = 415.
 * freq - for 300Hz - 16MHz / 256 / 300Hz = 208 - 1 = 207.
 * OCR1A = 415 or 207.
 * OCR1B = 125 for 30% duty cycle for 150Hz and 62 for 30% duty cycle for 300Hz.
 * (T = 6.666ms, pulse width t1 = 2ms, t2 = 4.666ms) for 150hz.
 * (T = 3.333ms, pulse width t1 = 1ms, t2 = 2.333ms) for 300hz.
 */
    void Transmitter::setUpTimer1() const {
        constexpr int PRESCALER = 256;
        TCCR1A = 0;
        TCCR1B = 0;                                    // ensure 0
        TCCR1A = _BV(WGM11) | _BV(WGM10);              // set mode 15 Fast PWM.
        TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS12);  // PRESCALER 256
        OCR1A = TGC::Transmitter::calcTimerFreq(this->desiredFreq, PRESCALER, TimerMode::FAST_PWM);
        OCR1B = TGC::Transmitter::calcTimerDutyCycle(this->dutyCycle, OCR1A);
        // enable Timer1 interrupts for OCR1B and OCR1A(debugging maybe).
        TIMSK1 = _BV(OCIE1B) | _BV(OCIE1A);
    }

/**
 * @brief Set up Timer 2.
 * Timer 2 is set up at pin 11(OC2A) and pin 3(OC2B)(Disabled, normal port).
 * Timer Mode 2 CTC, freq - 38KHz - 16MHz / 38KHz / 2 = 210 - 1 = 209.
 * OCR2A = 209.
 */
    void Transmitter::setUpTimer2() {
        constexpr double IR_FREQ = 38000;  // 38KHz
        constexpr int PRESCALER = 1;
        TCCR2A = 0;
        TCCR2B = 0;                         // ensure 0
        TCCR2A = _BV(COM2A0) | _BV(WGM21);  // set mode 2 CTC, OC2A and OC2B normal port operation.
        TCCR2B = _BV(CS20);                 // PRESCALER 1

        // Timer 2 38KHz - 16MHz / 38KHz / 2 = 210 - 1 = 209 CTC Mode.
        OCR2A = TGC::Transmitter::calcTimerFreq(IR_FREQ, PRESCALER, TimerMode::CTC);  // OCR2A = 209
    }

/**
 * @brief Calculate the timer OCRXA value.
 * Formula: Frequency(OCRxA) = F_CPU / 2 / N / (OCRxA + 1).
 */
    unsigned long Transmitter::calcTimerFreq(double freq, int prescaler, TimerMode timerMode) {
        unsigned long timerFreq = lround(F_CPU / static_cast<double>(prescaler) / freq);
        if (timerMode == TimerMode::CTC) {
            return timerFreq / 2;
        }
        return timerFreq - 1;
    }

/**
 * @brief Calculate the timer OCRXB value.
 * Formula: Duty cycle(OCRXB) = (OCR1B / (OCR1A) * 100.
 * To avoid divide by zero.
 */
    unsigned long Transmitter::calcTimerDutyCycle(int OCRXA, int dutyCycle) {
        return lround(OCRXA * (dutyCycle / 100.0));
    }
} // namespace TGC