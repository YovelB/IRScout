#include "BTConnection.hpp"

namespace GC {
    BTConnection::BTConnection() = default;

    void BTConnection::setup() {
        Serial1.begin(9600);
    }

    /**
     * @brief Check if the cart is turned on.
     * @return true if the cart is turned on, false otherwise.
     */
    bool BTConnection::isCartEnabled() {
        this->receiveMessage();
        return this->turnOnCart;
    }

    /**
     * @brief receive message from HC-06 module
     * This method reads the serial buffer and parse the received message.
     */
    void BTConnection::receiveMessage() {
        if (Serial1.available() > 0) {
            BTConnection::recWithStartEndMarkers();
        }
        if (this->newData) {
            BTConnection::parseData();
        }
    }

    /**
     * @brief Parse the received message.
     * This method checks the received message and performs the corresponding command.
     * The commands are:
     * - ON: turn on the led.
     * - OFF: turn off the led.
     */
    void BTConnection::parseData() {
        this->newData = false;
        if (this->receivedChars[0] == 'O' && this->receivedChars[1] == 'N') {
            this->turnOnCart = true;
        }
        if (this->receivedChars[0] == 'O' && this->receivedChars[1] == 'F') {
            this->turnOnCart = false;
        }
    }

    /**
     * @brief Receive message with start and end markers.
     * This method reads the serial buffer and stores the received message in the receivedChars array.
     * The start marker is '<' and the end marker is '>'.
     */
    void BTConnection::recWithStartEndMarkers() {
        static bool recInProgress = false;
        static byte ndx = 0;
        char startMarker = '<';
        char endMarker = '>';
        char rc;

        while (Serial1.available() > 0 && !this->newData) {
            rc = static_cast<char>(Serial1.read());
            if (recInProgress) {
                if (rc != endMarker) {
                    this->receivedChars[ndx] = rc;
                    ndx++;
                    if (ndx >= GC::BTConnection::numChars) {
                        ndx = GC::BTConnection::numChars - 1;
                    }
                } else {
                    this->receivedChars[ndx] = '\0';
                    recInProgress = false;
                    this->newData = true;
                    ndx = 0;
                }
            } else if (rc == startMarker) {
                recInProgress = true;
            }
        }
    }
}