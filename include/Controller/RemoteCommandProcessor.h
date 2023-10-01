#ifndef AIR_CONDITIONING_UNIT_REMOTECOMMANDPROCESSOR_H
#define AIR_CONDITIONING_UNIT_REMOTECOMMANDPROCESSOR_H

#include <Stream.h>
#include "Devices/Device.h"
#include "RemoteCommandRadio.h"

namespace ACC::Controller::RemoteCommand {
    /**
     * Wireless controller reads remote commands and reacts to them
     */
    class Processor {
        private:
            Stream &stream;
            Devices::Device &controlledDevice;

            /** Address to listen commands for */
            unsigned char listenAddress;
            /** Recognized command codes */
            static constexpr unsigned char turnOnCommand = 0x01U;
            static constexpr unsigned char turnOffCommand = 0x02U;
        public:
            explicit Processor(
                const Radio &radio,
                Devices::Device &controlledDevice,
                unsigned char listenAddress
            ) :
                stream(radio.getStream()),
                controlledDevice(controlledDevice),
                listenAddress(listenAddress) {}

            void process();
    };
}

#endif //AIR_CONDITIONING_UNIT_REMOTECOMMANDPROCESSOR_H
