#include <Arduino.h>
#include "unity.h"
#include "Transmitter.hpp"

// Use default params of getInstance(int signal=11, long desiredFreq=300, int dutyCycle=50)
TGC::Transmitter *transmitterIn300Hz = TGC::Transmitter::getInstance();
TGC::Transmitter *transmitterIn150Hz = TGC::Transmitter::getInstance(1, 150, 30);

//TODO set up test for frequency and duty cycle.
void test_setIROutput() {
    TGC::Transmitter *controller = TGC::Transmitter::getInstance(1, 150, 30);
    controller->setIROutput();

    //add tests here
}

void test_getInstance() {
    TEST_ASSERT_NOT_NULL(transmitterIn300Hz);
    TEST_ASSERT_NOT_NULL(transmitterIn150Hz);
}

void test_calcTimerFreq() {
    long result = transmitterIn300Hz->calcTimerFreq(38000, 1, TGC::Transmitter::TimerMode::CTC);
    TEST_ASSERT_EQUAL(209, result);

    result = transmitterIn150Hz->calcTimerFreq(300, 1, TGC::Transmitter::TimerMode::FAST_PWM);
    TEST_ASSERT_EQUAL(207, result);

    result = transmitterIn150Hz->calcTimerFreq(150, 1, TGC::Transmitter::TimerMode::FAST_PWM);
    TEST_ASSERT_EQUAL(415, result);
}

void test_calTimerDutyCycle() {
    long result = transmitterIn300Hz->calcTimerDutyCycle(30, 415);
    TEST_ASSERT_EQUAL(124, result);

    result = transmitterIn150Hz->calcTimerDutyCycle(30, 415);
    TEST_ASSERT_EQUAL(62, result);
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_setIROutput);
    RUN_TEST(test_getInstance);
    RUN_TEST(test_calcTimerFreq);
    RUN_TEST(test_calTimerDutyCycle);
    UNITY_END();
}

void loop() {}