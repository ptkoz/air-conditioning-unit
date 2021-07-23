#ifndef AIR_CONDITIONING_CONTROLLER_WIRELESSCONTROLLER_H
#define AIR_CONDITIONING_CONTROLLER_WIRELESSCONTROLLER_H

#include <Stream.h>
#include "Devices/AirConditioner.h"

namespace ACC::Controller {
    /**
     * Wireless controller reads remote commands and reacts to them. It can also sent commands to other devices.
     */
    class WirelessController {
        private:
            Stream & dataStream;
            unsigned char setPin;

            Devices::AirConditioner & airConditioner;

        public:
            explicit WirelessController(
                Stream & dataStream,
                unsigned char setPin,
                Devices::AirConditioner & airConditioner
            ):
                dataStream(dataStream),
                setPin(setPin),
                airConditioner(airConditioner) {}

            void initialize();
            void process();
    };
}

#endif //AIR_CONDITIONING_CONTROLLER_WIRELESSCONTROLLER_H
