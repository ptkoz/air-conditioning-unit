#pragma once

namespace ACC::Radio {
    /**
     * A class that represents and safely encodes an outbound message
     */
    class OutboundMessage {
        private:
            unsigned char *encodedBytes;

        public:
            OutboundMessage(
                unsigned char from,
                unsigned char to,
                unsigned char command,
                unsigned long nounce
            ) : OutboundMessage(from, to, command, nounce, nullptr, 0) {}

            OutboundMessage(
                unsigned char from,
                unsigned char to,
                unsigned char command,
                unsigned long nounce,
                const void *extended,
                unsigned char extendedLength
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
