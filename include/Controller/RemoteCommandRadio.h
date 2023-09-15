#ifndef AIR_CONDITIONING_RECEIVER_REMOTECOMMANDRADIO_H
#define AIR_CONDITIONING_RECEIVER_REMOTECOMMANDRADIO_H

#include <Stream.h>
#include "OutboundMessage.h"

namespace ACC::Controller::RemoteCommand {
    /**
     * Radio abstraction layer for processor & radio
     */
    class Radio {
        private:
            Stream & stream;
            const unsigned char setPin;

            void enterATMode();
            void leaveATMode();
        public:
            Radio(Stream & stream, unsigned char setPin):
                stream(stream),
                setPin(setPin) {}

            void initialize();
            void send(const OutboundMessage & message);

            Stream & getStream() const {
                return stream;
            }
    };
}

#endif //AIR_CONDITIONING_CONTROLLER_REMOTECOMMANDRADIO_H
