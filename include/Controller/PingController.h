#pragma once

#include "Time/Source.h"
#include "Radio/Publisher.h"
#include "Devices/Device.h"
#include "Security/NounceProvider.h"

namespace ACC::Controller {
    /**
     * Controller that periodically sends ping to master unit
     */
    class PingController {
        private:
            /** How often do we send the ping? */
            static constexpr unsigned short interval = 60; //seconds

            /** Reliable time source */
            const Time::Source &timeSource;

            /** Transport for the ping messages */
            Radio::Publisher &publisher;

            /** Device that is being controlled */
            Devices::Device &controlledDevice;

            /** Where we send pings to? */
            const unsigned char sendToAddress;

            /** What is the ping command? */
            const unsigned char pingCommand;

            /** Timestamp of last sent ping (initialize with negative value to send immediately on power up */
            Time::Timestamp lastPing = Time::Timestamp(-interval);

        public:
            PingController(
                const Time::Source &timeSource,
                Radio::Publisher &publisher,
                Devices::Device &controlledDevice,
                unsigned char sendToAddress,
                unsigned char pingCommand
            ) :
                timeSource(timeSource),
                publisher(publisher),
                controlledDevice(controlledDevice),
                sendToAddress(sendToAddress),
                pingCommand(pingCommand) {}

            void process();

    };
}
