#pragma once

namespace GC {
    class MovingAverage {
    public:
        explicit MovingAverage(int windowSize, double minVal, double maxVal);
        ~MovingAverage();

        void add(double val);
        double average() const;

    private:
        int windowSize{};
        double minVal{}, maxVal{};
        double *buffer{};
        double sum{0.0};
        int index{0};
    };
} // namespace GC