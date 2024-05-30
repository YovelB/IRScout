#pragma once

#include <Arduino.h>

#include "MotorControl.hpp"
#include "IRReceiver.hpp"
#include "BTConnection.hpp"
#include "DistanceSensor.hpp"

namespace GC {
    enum class CartState {
        MOVE_FORWARD,
        SEARCH_IR,
        BLOCKED
    };

    class GuidedCart {
    public:
        explicit GuidedCart();

        void setup();
        void loop();

    private:
        // current state of the cart.
        CartState currState{CartState::MOVE_FORWARD};

        // IR signal settings
        static constexpr const double FREQUENCY{150};
        static constexpr const double DUTY_CYCLE{70};

        // Distance sensor settings
        static constexpr const double FRONT_THRESHOLD{30};
        static constexpr const double SIDE_THRESHOLD{13};

        // IR receiver
        static constexpr const byte IR_PIN{42};
        GC::IRReceiver irReceiver;

        // Motor control
        // duration for rotating in place for searching the IR signal for each side, hence 3000.
        static constexpr const int IR_SEARCH_DURATION_MS{1400};
        // duration for rotating when the cart is blocked. (check that it is 50).
        static constexpr const int BLOCKED_ROTATE_DURATION_MS{ROTATE_DELAY_MS};
        // speed in the MOVE_FORWARD and BLOCKED states because it is easier to move around the object at a higher speed.
        static constexpr const byte CART_MOVE_VELOCITY{120};
        // speed in the SEARCH_IR state because it is easier to detect the IR signal at a lower speed.
        static constexpr const byte CART_ROTATE_VELOCITY{100};
        GC::MotorControl motorControl;

        // bluetooth connection
        GC::BTConnection btConnection;

        // Distance sensors
        static constexpr const byte RIGHT_TRIG_PIN{50}, RIGHT_ECHO_PIN{48};
        static constexpr const byte FRONT_TRIG_PIN{34}, FRONT_ECHO_PIN{32};
        static constexpr const byte LEFT_TRIG_PIN{26}, LEFT_ECHO_PIN{24};
        GC::DistanceSensor sensorFront;
        GC::DistanceSensor sensorLeft;
        GC::DistanceSensor sensorRight;

        // MOVE_FORWARD state
        /**
         * @brief Move the cart forward if it is not blocked.
         *
         * Moves forward if none of the sensors are blocked.
         * When done moving for MOVE_DELAY_MS, it toggles to SEARCH_IR state.
         * Otherwise, stops the cart and toggle to BLOCKED state.
         * When toggling to the state,
         * it sets the rotate timer to BLOCKED_ROTATE_DURATION_MS.
         * and sets the speed to CART_ROTATE_VELOCITY.
         */
        void moveForward();
        void toggleToBlocked();
        void toggleToSearchIR();

        // SEARCH_IR state
        /**
         * @brief Search for the IR signal.
         *
         * If the IR signal is found, then move towards the signal.
         * Otherwise, rotate right then left to search for the IR signal.
         * It toggles to MOVE_FORWARD state if the IR signal is found.
         * Otherwise, after a duration of IR_SEARCH_DURATION_MS, it toggles to MOVE_FORWARD state.
         */
        void searchIRSignal();
        void toggleFromSearchIR();

        // BLOCKED state
        /**
         * @brief Maneuver around an object.
         *
         * Rotates the cart to the left or right based on the side sensors.
         * If both side sensors are blocked, then it rotates based on the greater distance.
         * If the right sensor is blocked, then it rotates to the left.
         * Otherwise, if the left sensor is blocked or only the front sensor is blocked, then it rotates to the right.
         */
        void rotateWhenBlocked();
        /**
        * @brief move the cart based on the greater distance.
         *
         * This method checks the distance from the front, left, and right sensors.
         * If the front sensor is blocked, then it checks the side sensors.
         * If both side sensors are blocked, then it moves based on the greater distance.
         * If the right sensor is blocked, then it moves around the object to the left.
         * Otherwise, it moves around the object to the right.
         */
        void moveBasedOnGreaterDistance();
        /**
         * @brief Rotate the cart to search for the IR signal.
         *
         * This method rotates the cart to the left then to the right.
         * It does it every IR_SEARCH_EVERY_MS, for a duration of IR_SEARCH_DURATION_MS.
         */
        void RotateToSearch();
        void toggleFromBlocked();
    };
}
