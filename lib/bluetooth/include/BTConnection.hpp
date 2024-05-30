#pragma once

#include <Arduino.h>

namespace GC {
    class BTConnection {
    public:
        /**
         * Constructor for the BTConnection class.
         * @param rxPin - arduino pin connected to HC-06 TX pin.
         * @param txPin - arduino pin connected to HC-06 RX pin, through a voltage divider.
         */
        explicit BTConnection();

        void setup();
        bool isCartEnabled();

    private:
        bool turnOnCart{false};

        static constexpr int numChars{32};
        char receivedChars[numChars]{};
        bool newData{false};

        void recWithStartEndMarkers();
        void parseData();
        void receiveMessage();
    };
} // namespace GC