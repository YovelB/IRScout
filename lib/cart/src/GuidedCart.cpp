#include "GuidedCart.hpp"

namespace GC {
    GuidedCart::GuidedCart() : irReceiver(IR_PIN), motorControl(CART_MOVE_VELOCITY),
                               sensorFront(FRONT_TRIG_PIN, FRONT_ECHO_PIN),
                               sensorLeft(LEFT_TRIG_PIN, LEFT_ECHO_PIN),
                               sensorRight(RIGHT_TRIG_PIN, RIGHT_ECHO_PIN) {}

    void GuidedCart::setup() {
        // initialize the objects.
        irReceiver.setup();
        btConnection.setup();
        motorControl.setup();
        sensorFront.setup();
        sensorLeft.setup();
        sensorRight.setup();
    }

    void GuidedCart::loop() {
        if (btConnection.isCartEnabled()) {
            switch (this->currState) {
                case CartState::MOVE_FORWARD:moveForward();
                    break;
                case CartState::SEARCH_IR:searchIRSignal();
                    break;
                case CartState::BLOCKED:rotateWhenBlocked();
                    break;
            }
        } else {
            motorControl.moveDir(DIR_STOP);
        }
    }

    // MOVE_FORWARD state.
    void GuidedCart::moveForward() {
        if (sensorFront.isBlocked(FRONT_THRESHOLD) ||
            sensorLeft.isBlocked(SIDE_THRESHOLD) || sensorRight.isBlocked(SIDE_THRESHOLD)) {
            toggleToBlocked();
        } else {
            if (motorControl.moveDir(DIR_FORWARD)) {
                toggleToSearchIR();
            }
        }
    }
    void GuidedCart::toggleToBlocked() {
        motorControl.moveDir(DIR_STOP, false);
        currState = CartState::BLOCKED;
    }
    void GuidedCart::toggleToSearchIR() {
        motorControl.moveDir(DIR_STOP, false);
        motorControl.setRotateTimerDelay(IR_SEARCH_DURATION_MS, true);
        motorControl.setCartVelocity(CART_ROTATE_VELOCITY);
        currState = CartState::SEARCH_IR;
    }

    // SEARCH_IR state.
    void GuidedCart::searchIRSignal() {
        if (irReceiver.isCorrectIRSignal(FREQUENCY, DUTY_CYCLE)) {
            toggleFromSearchIR();
        } else {
            RotateToSearch();
        }
    }
    void GuidedCart::RotateToSearch() {
        static bool isLeft = true;
        if (isLeft) {
            if (motorControl.moveDir(DIR_LEFT)) {
                motorControl.moveDir(DIR_STOP, false);
                isLeft = false;
            }
        } else {
            if (motorControl.moveDir(DIR_RIGHT)) {
                toggleFromSearchIR();
                isLeft = true;
            }
        }
    }
    void GuidedCart::toggleFromSearchIR() {
        motorControl.moveDir(DIR_STOP, false);
        motorControl.setRotateTimerDelay(BLOCKED_ROTATE_DURATION_MS, true);
        motorControl.setCartVelocity(CART_MOVE_VELOCITY);
        currState = CartState::MOVE_FORWARD;
    }

    // BLOCKED state.
    void GuidedCart::rotateWhenBlocked() {
        if (sensorLeft.isBlocked(SIDE_THRESHOLD) && sensorRight.isBlocked(SIDE_THRESHOLD)) {
            moveBasedOnGreaterDistance();
        } else if (sensorRight.isBlocked(SIDE_THRESHOLD)) {
            if (motorControl.moveDir(DIR_LEFT)) {
                toggleFromBlocked();
            }
        } else {
            if (motorControl.moveDir(DIR_RIGHT)) {
                toggleFromBlocked();
            }
        }
    }
    void GuidedCart::moveBasedOnGreaterDistance() {
        if (sensorLeft.getDistance() > sensorRight.getDistance()) {
            if (motorControl.moveDir(DIR_LEFT)) {
                toggleFromBlocked();
            }
        } else {
            if (motorControl.moveDir(DIR_RIGHT)) {
                toggleFromBlocked();
            }
        }
    }
    void GuidedCart::toggleFromBlocked() {
        motorControl.moveDir(DIR_STOP, false);
        currState = CartState::MOVE_FORWARD;
    }
} // namespace GC