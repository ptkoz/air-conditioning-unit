#ifndef AIR_CONDITIONING_UNIT_OUTBOUNDMESSAGE_H
#define AIR_CONDITIONING_UNIT_OUTBOUNDMESSAGE_H

namespace ACC::Controller::RemoteCommand {
    /**
     * A class that represents and safely encodes an outbound message
     */
    class OutboundMessage {
        private:
            unsigned char * encodedBytes;

        public:
            OutboundMessage(
                unsigned char from,
                unsigned char to,
                unsigned char command,
                unsigned long nounce,
                const void *extended = nullptr,
                unsigned char extendedLength = 0
            );

            ~OutboundMessage();

            const unsigned char *getEncodedBytes() const {
                return encodedBytes;
            }

            unsigned char getLength() const {
                return encodedBytes[1] + 2; // the actual message bytes + start marker byte + length byte
            }
    };
}

#endif //AIR_CONDITIONING_UNIT_OUTBOUNDMESSAGE_H
