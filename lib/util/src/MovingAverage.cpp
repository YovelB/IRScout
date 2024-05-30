#include "MovingAverage.hpp"

namespace GC {
    MovingAverage::MovingAverage(int windowSize, double minVal, double maxVal)
        : windowSize(windowSize), minVal(minVal), maxVal(maxVal) {
        buffer = new double[windowSize];
        for (int i = 0; i < windowSize; ++i) {
            buffer[i] = 0.0;
        }
    }

    MovingAverage::~MovingAverage() {
        delete[] buffer;
    }

    void MovingAverage::add(double val) {
        if (val < minVal || val > maxVal) {
            // Ignore readings that are out of range.
            return;
        }
        // Subtract the old value from the sum and add the new value.
        sum = sum - buffer[index] + val;
        // Replace the old value with the new value.
        buffer[index] = val;

        // Increment the index and wrap around to 0 if necessary.
        index = (index + 1) % windowSize;
    }

    double MovingAverage::average() const {
        return sum / windowSize;
    }
} // namespace GC