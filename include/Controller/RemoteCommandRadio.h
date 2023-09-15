#ifndef AIR_CONDITIONING_RECEIVER_REMOTECOMMANDRADIO_H
#define AIR_CONDITIONING_RECEIVER_REMOTECOMMANDRADIO_H

#include <Stream.h>

namespace ACC::Controller::RemoteCommand {
    /**
     * Radio abstraction layer for processor & executor
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

            Stream & getStream() const {
                return stream;
            }

            /**
             * Encodes message into the output for serial transmission. Please mind output needs to be exactly
             * twice the size of the data.
             */
            void encode(unsigned char * output, const unsigned char * data, size_t dataLength) const;

            /**
             * Decodes message received through serial into the output. Please mind data needs to be exactly twice
             * as long as the message output.
             */
            void decode(unsigned char * output, const unsigned char * data, size_t outputLength) const;
    };
}

#endif //AIR_CONDITIONING_CONTROLLER_REMOTECOMMANDRADIO_H
