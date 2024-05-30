#include "MotorControl.hpp"

namespace GC {
    MotorControl::MotorControl(int velocity, int M1, int M2, int M3, int M4) :
            velocity(velocity),
            leftMotors{AF_DCMotor(M1), AF_DCMotor(M2)},
            rightMotors{AF_DCMotor(M3), AF_DCMotor(M4)} {}

    void MotorControl::setup(int moveDelay, int rotateDelay, int stopDelay) {
        // set up timers
        moveTimer = BlockNot(moveDelay, STOPPED);
        rotateTimer = BlockNot(rotateDelay, STOPPED);
        stopTimer = BlockNot(stopDelay, STOPPED);

        setMotorCommand(RELEASE, RELEASE);
        setCartVelocity(this->velocity);
    }

    bool MotorControl::moveDir(Direction nextDir, bool withDelay) {
        if (currentDir != nextDir) {
            moveTimer.stop();
            rotateTimer.stop();
            stopTimer.stop();
            currentDir = nextDir;
        }
        bool isDone = false;
        switch (currentDir) {
            case DIR_FORWARD:isDone = moveForward(withDelay);
                break;
            case DIR_LEFT:isDone = rotateLeft(withDelay);
                break;
            case DIR_RIGHT:isDone = rotateRight(withDelay);
                break;
            case DIR_STOP:isDone = stop(withDelay);
                break;
        }
        return isDone;
    }

    bool MotorControl::moveForward(bool withDelay) {
        if (withDelay) {
            if (moveTimer.isStopped()) {
                moveTimer.start(WITH_RESET);
                setMotorCommand(FORWARD, FORWARD);
            } else {
                if (moveTimer.triggered()) {
                    moveTimer.stop();
                    return true;
                }
            }
        } else {
            setMotorCommand(FORWARD, FORWARD);
        }
        return false;
    }

    bool MotorControl::rotateLeft(bool withDelay) {
        if (withDelay) {
            if (rotateTimer.isStopped()) {
                rotateTimer.start(WITH_RESET);
                setMotorCommand(BACKWARD, FORWARD);
            } else {
                if (rotateTimer.triggered()) {
                    rotateTimer.stop();
                    return true;
                }
            }
        } else {
            setMotorCommand(BACKWARD, FORWARD);
        }
        return false;
    }

    bool MotorControl::rotateRight(bool withDelay) {
        if (withDelay) {
            if (rotateTimer.isStopped()) {
                rotateTimer.start(WITH_RESET);
                setMotorCommand(FORWARD, BACKWARD);
            } else {
                if (rotateTimer.triggered()) {
                    rotateTimer.stop();
                    return true;
                }
            }
        } else {
            setMotorCommand(FORWARD, BACKWARD);
        }
        return false;
    }

    bool MotorControl::stop(bool withDelay) {
        if (withDelay) {
            if (stopTimer.isStopped()) {
                stopTimer.start(WITH_RESET);
                setMotorCommand(RELEASE, RELEASE);
            } else {
                if (stopTimer.triggered()) {
                    stopTimer.stop();
                    return true;
                }
            }
        } else {
            setMotorCommand(RELEASE, RELEASE);
        }
        return false;
    }

    void MotorControl::setCartVelocity(int newVelocity) {
        this->velocity = newVelocity;
        forEachMotor(
                [this](AF_DCMotor &motor) { motor.setSpeed(this->velocity); },
                [this](AF_DCMotor &motor) { motor.setSpeed(this->velocity); }
        );
    }

    void MotorControl::setMotorCommand(int cmdLeft, int cmdRight) {
        forEachMotor(
                [cmdLeft](AF_DCMotor &motor) { motor.run(cmdLeft); },
                [cmdRight](AF_DCMotor &motor) { motor.run(cmdRight); }
        );
    }
} // namespace GC