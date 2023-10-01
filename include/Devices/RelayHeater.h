#ifndef AIR_CONDITIONING_UNIT_RELAYHEATER_H
#define AIR_CONDITIONING_UNIT_RELAYHEATER_H

#include <IRremoteInt.h>
#include "Device.h"
#include "Time/Source.h"

namespace ACC::Devices {
    /**
     * A heater controlled by a simple relay transmitter.
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

#endif //AIR_CONDITIONING_UNIT_RELAYHEATER_H
