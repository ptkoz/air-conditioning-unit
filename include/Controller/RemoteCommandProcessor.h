#ifndef AIR_CONDITIONING_RECEIVER_REMOTECOMMANDPROCESSOR_H
#define AIR_CONDITIONING_RECEIVER_REMOTECOMMANDPROCESSOR_H

#include <Stream.h>
#include "Devices/AirConditioner.h"
#include "RemoteCommandExecutor.h"

namespace ACC::Controller::RemoteCommand {
    /**
     * Wireless controller reads remote commands and reacts to them. It can also sent commands to other devices.
     */
    class Processor : public Executor {
        private:
            /** Address to listen commands for */
            static constexpr unsigned short listenAddress = 0xA2;
            /** Recognized command codes */
            static constexpr unsigned short turnOnCommand = 0x01;
            static constexpr unsigned short turnOffCommand = 0x02;
            static constexpr unsigned short setLowSpeedCommand = 0x03;
            static constexpr unsigned short setHighSpeedCommand = 0x04;

            Stream & stream;
            unsigned char setPin;

            Devices::AirConditioner & airConditioner;

            void enterATMode();
            void leaveATMode();
        public:
            explicit Processor(
                Stream & stream,
                unsigned char setPin,
                Devices::AirConditioner & airConditioner
            ):
                stream(stream),
                setPin(setPin),
                airConditioner(airConditioner) {}

            void initialize();
            void process();

            void execute(
                unsigned short address,
                unsigned short command,
                const void * message,
                size_t length
            ) override;
    };
}

#endif //AIR_CONDITIONING_RECEIVER_REMOTECOMMANDPROCESSOR_H
