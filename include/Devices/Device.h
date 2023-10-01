#ifndef AIR_CONDITIONING_UNIT_DEVICE_H
#define AIR_CONDITIONING_UNIT_DEVICE_H

namespace ACC::Devices {
    /**
     * Interface for a controlled device
     */
    class Device {
        public:
            /** Initialize device */
            virtual void initialize() {};
            /** Tries to turn on the AC */
            virtual bool turnOn() = 0;
            /** Tries to turn off the AC */
            virtual bool turnOff() = 0;
            /** Whether the device is turned on or off */
            virtual bool isWorking() = 0;
    };
}

#endif //AIR_CONDITIONING_UNIT_DEVICE_H
