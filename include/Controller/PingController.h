#ifndef AIR_CONDITIONING_RECEIVER_PINGCONTROLLER_H
#define AIR_CONDITIONING_RECEIVER_PINGCONTROLLER_H

#include "Time/Source.h"
#include "RemoteCommandRadio.h"

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

            /** Executor of the remote commands */
            RemoteCommand::Radio & radio;

            /** Where to send ping? */
            const unsigned short recipientAddress;

            /** What is the ping command? */
            const unsigned short recipientCommand;

            /** Timestamp of last sent ping (initialize with negative value to send immediately on power up */
            Time::Timestamp lastPing = Time::Timestamp(-interval);

        public:
            PingController(
                const Time::Source & timeSource,
                RemoteCommand::Radio & radio,
                unsigned short recipientAddress,
                unsigned short recipientCommand
            ):
                timeSource(timeSource),
                radio(radio),
                recipientAddress(recipientAddress),
                recipientCommand(recipientCommand) {}

            void process();
    };
}

#endif //AIR_CONDITIONING_RECEIVER_PINGCONTROLLER_H
