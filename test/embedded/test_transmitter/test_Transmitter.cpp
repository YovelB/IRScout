#include "unity.h"
#include "Transmitter.hpp"

TGC::Transmitter *transmitterIn300Hz = TGC::Transmitter::getInstance();
TGC::Transmitter *transmitterIn150Hz = TGC::Transmitter::getInstance(1, 150, 30);

//TODO set up test for frequency and duty cycle.
void test_setIROutput() {
    TGC::Transmitter *controller = TGC::Transmitter::getInstance(1, 150, 30);
    controller->setIROutput();

    //add tests here
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_setIROutput);
    UNITY_END();
    return 0;
}