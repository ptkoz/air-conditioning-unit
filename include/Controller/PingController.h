#ifndef AIR_CONDITIONING_UNIT_PINGCONTROLLER_H
#define AIR_CONDITIONING_UNIT_PINGCONTROLLER_H

#include "Time/Source.h"
#include "RemoteCommandRadio.h"
#include "Devices/Device.h"

namespace ACC::Controller {
    /**
     * Class that periodically sends ping to master unit
     */
    class PingController {
        private:
            /** How often do we send the ping? */
            static constexpr unsigned short interval = 60; //seconds

            /** Reliable time source */
            const Time::Source & timeSource;

            /** Transport for the ping messages */
            RemoteCommand::Radio & radio;

            /** Device that is being controlled */
            Devices::Device & controlledDevice;

            /** Where are the pings sent from? */
            const unsigned char fromAddress;

            /** Where we send pings to? */
            const unsigned char toAddress;

            /** What is the ping command? */
            const unsigned char pingCommand;

            /** Timestamp of last sent ping (initialize with negative value to send immediately on power up */
            Time::Timestamp lastPing = Time::Timestamp(-interval);

        public:
            PingController(
                const Time::Source & timeSource,
                RemoteCommand::Radio & radio,
                Devices::Device & controlledDevice,
                unsigned char fromAddress,
                unsigned char toAddress,
                unsigned char pingCommand
            ):
                timeSource(timeSource),
                radio(radio),
                controlledDevice(controlledDevice),
                fromAddress(fromAddress),
                toAddress(toAddress),
                pingCommand(pingCommand) {}

            void process();

    };
}

#endif //AIR_CONDITIONING_UNIT_PINGCONTROLLER_H
