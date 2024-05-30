#pragma once

#include <Arduino.h>
#include <AFMotor.h>
#include <BlockNot.h>

namespace GC {

    // Define the delay for each movement.
    static constexpr const int MOVE_DELAY_MS = 1500;
    static constexpr const int ROTATE_DELAY_MS = 25;
    static constexpr const int STOP_DELAY_MS = 100;

    // Define the direction of the cart.
    enum Direction {
        DIR_FORWARD, DIR_LEFT, DIR_RIGHT, DIR_STOP
    };

    class MotorControl {
    public:
        explicit MotorControl(int velocity = 250, int M1 = 1, int M2 = 2, int M3 = 3, int M4 = 4);
        void setup(int moveDelay = MOVE_DELAY_MS, int rotateDelay = ROTATE_DELAY_MS, int stopDelay = STOP_DELAY_MS);

        /**
         * @brief Move the cart in a specific direction.
         * @param nextDir - direction to move the cart.
         * @param withDelay - apply movement only for a specific time.
         * @return true if the movement is done (only when withDelay is true) else false.
         */
        bool moveDir(Direction nextDir, bool withDelay = true);

        void setCartVelocity(int newVelocity);
        int getCartVelocity() const { return this->velocity; }

        /**
         * @brief Set the delay for rotating the cart.
         * @param delay - the delay in milliseconds.
         * @param withReset - true to reset the timer, false otherwise.
         */
        void setRotateTimerDelay(unsigned long delay, bool withReset = true) {
            rotateTimer.setDuration(delay, withReset);
        }

    private:
        int velocity{};
        Direction currentDir{DIR_STOP};

        // create an Array of AF_DCMotor objects and initialize them
        AF_DCMotor leftMotors[2];
        AF_DCMotor rightMotors[2];

        // create timers for each movement.
        BlockNot moveTimer;
        BlockNot rotateTimer;
        BlockNot stopTimer;

        bool moveForward(bool withDelay);
        bool rotateLeft(bool withDelay);
        bool rotateRight(bool withDelay);
        bool stop(bool withDelay);

        /**
         * @brief Set the motor command for each motor.
         * @param cmdLeft - command for the left motors.
         * @param cmdRight - command for the right motors.
         */
        void setMotorCommand(int cmdLeft, int cmdRight);

        /**
         * @brief Apply a function to each motor.
         * @param funcLeft - function to apply to the left motors.
         * @param funcRight - function to apply to the right motors.
         */
        template<typename FuncLeft, typename FuncRight>
        void forEachMotor(FuncLeft funcLeft, FuncRight funcRight) {
            for (auto &leftMotor: leftMotors) {
                funcLeft(leftMotor);
            }
            for (auto &rightMotor: rightMotors) {
                funcRight(rightMotor);
            }
        }
    };
} // namespace GC