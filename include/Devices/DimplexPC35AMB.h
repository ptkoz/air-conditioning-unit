#ifndef AIR_CONDITIONING_UNIT_DIMPLEXPC35AMB_H
#define AIR_CONDITIONING_UNIT_DIMPLEXPC35AMB_H

#include <IRremoteInt.h>
#include "Device.h"
#include "Time/Source.h"

namespace ACC::Devices {
    /**
     * Air conditioner implementation for Dimplex device
     */
    class DimplexPC35AMB : public Device {
        private:
            /** IR Emitter frequency */
            static constexpr unsigned char irFrequency = 38;

            static constexpr unsigned short turnOnGracePeriodSeconds = 210;

            /** Digital pin for IR communication */
            const unsigned char irPin;

            /** IR emitter that sends signals to the device */
            IRsend & irEmitter;

            /** Reliable source of time */
            const Time::Source & timeSource;

            /** The timestamp of the last turn off */
            Time::Timestamp lastTurnOffTimestamp;

            bool isTurnedOn;
        public:
            explicit DimplexPC35AMB(unsigned char irPin, const Time::Source & timeSource);

            /**
             * Initializes the Device
             */
            void initialize() {
                irEmitter.begin(irPin, false);
            }

            /**
             * Turns on the device on. According to User Manual, the device cannot be turned on before 3min and 30sec
             * passes since the last Turn Off. This grace period is required to cool down the compressor.
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

            /**
             * Tries to set AC to low speed
             */
            bool setLowSpeed();

            /**
             * Tries to set AC to high speed
             */
            bool setHighSpeed();
    };
}

#endif //AIR_CONDITIONING_UNIT_DIMPLEXPC35AMB_H
