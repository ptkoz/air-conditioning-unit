#ifndef AIR_CONDITIONING_SENSOR_OUTBOUNDMESSAGE_H
#define AIR_CONDITIONING_SENSOR_OUTBOUNDMESSAGE_H

namespace ACC::Controller::RemoteCommand {
    /**
     * A class that represents and safely encodes an outbound message
     */
    class OutboundMessage {
        private:
            unsigned char *encodedData;

        public:
            OutboundMessage(
                unsigned char address,
                unsigned char command,
                const void *message = nullptr,
                unsigned char messageLength = 0
            );

            ~OutboundMessage();

            const unsigned char *getEncodedData() const {
                return encodedData;
            }

            unsigned char getDataLength() const {
                return encodedData[1] + 2; // the actual message + start marker + length
            }
    };
}

#endif //AIR_CONDITIONING_SENSOR_OUTBOUNDMESSAGE_H
