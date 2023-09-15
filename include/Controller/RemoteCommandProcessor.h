#ifndef AIR_CONDITIONING_RECEIVER_REMOTECOMMANDPROCESSOR_H
#define AIR_CONDITIONING_RECEIVER_REMOTECOMMANDPROCESSOR_H

#include <Stream.h>
#include "Devices/AirConditioner.h"
#include "RemoteCommandRadio.h"

namespace ACC::Controller::RemoteCommand {
    /**
     * Wireless controller reads remote commands and reacts to them
     */
    class Processor {
        private:
            /** Address to listen commands for */
            static constexpr unsigned short listenAddress = 0xA2;
            /** Recognized command codes */
            static constexpr unsigned short turnOnCommand = 0x01;
            static constexpr unsigned short turnOffCommand = 0x02;
            static constexpr unsigned short setLowSpeedCommand = 0x03;
            static constexpr unsigned short setHighSpeedCommand = 0x04;

            Stream & stream;
            const Radio & radio;
            Devices::AirConditioner & airConditioner;
        public:
            explicit Processor(
                const Radio & radio,
                Devices::AirConditioner & airConditioner
            ):
                stream(radio.getStream()),
                radio(radio),
                airConditioner(airConditioner) {}

            void process();
    };
}

#endif //AIR_CONDITIONING_RECEIVER_REMOTECOMMANDPROCESSOR_H
