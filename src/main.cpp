#include <Arduino.h>

#include "GuidedCart.hpp"

GC::GuidedCart guidedCart;

void setup() {
    guidedCart.setup();
}

void loop() {
    guidedCart.loop();
}
