#ifndef AIR_CONDITIONING_UNIT_INBOUNDMESSAGE_H
#define AIR_CONDITIONING_UNIT_INBOUNDMESSAGE_H

namespace ACC::Controller::RemoteCommand {
    /**
     * A class that represents and safely decodes an inbound message
     */
    class InboundMessage {
        private:
            unsigned char recipient;
            unsigned char command;
            unsigned char *data;
            unsigned char dataLength;

        public:
            InboundMessage(const unsigned char *input, unsigned char inputLength);
            ~InboundMessage();

            unsigned char getRecipient() const { return recipient; }
            unsigned char getCommand() const { return command; }
            const unsigned char * getData() const { return data; }
            unsigned char getDataLength() const { return dataLength; }
    };
}

#endif //AIR_CONDITIONING_UNIT_INBOUNDMESSAGE_H
