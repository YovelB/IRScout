#include "DistanceSensor.hpp"

namespace GC {
    DistanceSensor::DistanceSensor(int trigPin, int echoPin)
            : trigPin(trigPin), echoPin(echoPin) {}

    void DistanceSensor::setup() const {
        pinMode(this->trigPin, OUTPUT);
        pinMode(this->echoPin, INPUT);
    }

    bool DistanceSensor::isBlocked(double threshold) {
        this->measure();
        return this->distance <= threshold;
    }

    double DistanceSensor::measure() {
        const double SPEED_OF_SOUND = 0.0344;

        digitalWrite(trigPin, LOW);
        delayMicroseconds(2); // wait 2 us to avoid collisions in serial monitor.
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10); // keep the trigPin high for 10 us.
        digitalWrite(trigPin, LOW);
        // measure the duration of the echoPin.
        this->duration = pulseIn(echoPin, HIGH);

        this->distance = static_cast<double>(duration) * SPEED_OF_SOUND / 2;
        return this->distance;
    }
} // namespace GC