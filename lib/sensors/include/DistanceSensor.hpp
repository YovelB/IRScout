#pragma once

#include <Arduino.h>
#include "MovingAverage.hpp"

namespace GC {
    class DistanceSensor {
    public:
        explicit DistanceSensor(int trigPin, int echoPin);
        void setup() const;

        /**
         * @brief Check if the sensor is blocked.
         * @param threshold - the distance threshold in cm.
         * @return true if the sensor is blocked, false otherwise.
         */
        bool isBlocked(double threshold);

        unsigned long getDuration() const { return duration; }
        double getDistance() const { return distance; }

    private:
        int trigPin{}, echoPin{};
        unsigned long duration{}; // duration in microseconds.
        double distance{}; // distance in cm.

        /**
         * @brief Measure the sensor output and calculate the distance in cm.
         *
         * Turn on the trigPin for 10 us and measure the duration of the echoPin.
         * Technical details:
         * The speed of sound in air is 340 m/s or 0.034 cm/us.
         * The distance is half of the duration multiplied by the speed of sound.
         * time = distance / speed
         * time = (10/0.034)us = 294us
         * distance = (speed * time) / 2
         * distance = (0.034 * 294) / 2 = 5 cm
         * @return The distance in cm.
         */
        double measure();
    };
} // namespace GC