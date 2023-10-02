#pragma once

namespace ACC::Devices {
    /**
     * Interface for a controlled device
     */
    class Device {
        public:
            /** Initialize device */
            virtual void initialize() {};
            /** Tries to turn on the device */
            virtual bool turnOn() = 0;
            /** Tries to turn off the device */
            virtual bool turnOff() = 0;
            /** Whether the device is turned on or off */
            virtual bool isWorking() = 0;
    };
}
