#pragma once

#include "Device.h"

namespace ACC::Devices {
    /**
     * A device implementation for a heater controlled by a simple relay transmitter.
     */
    class RelayHeater : public Device {
        private:
            unsigned char relayPin;
            bool isTurnedOn;
        public:
            explicit RelayHeater(unsigned char relayPin);

            /**
             * Turns the device on
             */
            bool turnOn() override;

            /**
             * Turns off the device
             */
            bool turnOff() override;

            /**
             * Checks whether device is currently working
             */
            bool isWorking() override {
                return isTurnedOn;
            }

    };
}
