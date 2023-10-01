#ifndef AIR_CONDITIONING_UNIT_INBOUNDMESSAGE_H
#define AIR_CONDITIONING_UNIT_INBOUNDMESSAGE_H

namespace ACC::Controller::RemoteCommand {
    /**
     * A class that represents and safely decodes an inbound message
     */
    class InboundMessage {
        private:
            unsigned char fromAddress;
            unsigned char toAddress;
            unsigned char command;
            unsigned long nounce;
            bool isHmacValid;
            unsigned char *extendedBytes;
            unsigned char extendedBytesLength;

        public:
            InboundMessage(const unsigned char *input, unsigned char inputLength);

            ~InboundMessage();

            unsigned char getFromAddress() const { return fromAddress; }

            unsigned char getToAddress() const { return toAddress; }

            unsigned char getCommand() const { return command; }

            const unsigned char *getExtendedBytes() const { return extendedBytes; }

            unsigned char getExtendedBytesLength() const { return extendedBytesLength; }

            bool isValid(unsigned long lastInputNounce) const;
    };
}

#endif //AIR_CONDITIONING_UNIT_INBOUNDMESSAGE_H
