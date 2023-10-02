#pragma once

#include "Device.h"

namespace ACC::Devices {
    /**
     * A device implementation for a heater controlled by a simple relay transmitter.
     */
    class RelayHeater : public Device {
        private:
            bool isTurnedOn;
        public:
            explicit RelayHeater();

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
