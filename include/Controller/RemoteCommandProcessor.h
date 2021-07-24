#ifndef AIR_CONDITIONING_CONTROLLER_REMOTECOMMANDPROCESSOR_H
#define AIR_CONDITIONING_CONTROLLER_REMOTECOMMANDPROCESSOR_H

#include <Stream.h>
#include "Devices/AirConditioner.h"

namespace ACC::Controller {
    /**
     * Wireless controller reads remote commands and reacts to them. It can also sent commands to other devices.
     */
    class RemoteCommandProcessor {
        private:
            /** Address to listen commands for */
            static constexpr unsigned short listenAddress = 0xA2;
            /** Recognized command codes */
            static constexpr unsigned short turnOnCommand = 0x01;
            static constexpr unsigned short turnOffCommand = 0x02;

            Stream & dataStream;
            unsigned char setPin;

            Devices::AirConditioner & airConditioner;

        public:
            explicit RemoteCommandProcessor(
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

#endif //AIR_CONDITIONING_CONTROLLER_REMOTECOMMANDPROCESSOR_H
