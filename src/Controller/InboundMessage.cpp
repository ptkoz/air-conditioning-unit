#include "Controller/InboundMessage.h"
#include "Arduino.h"

using namespace ACC::Controller::RemoteCommand;

InboundMessage::InboundMessage(const unsigned char *input, unsigned char inputLength) {
    unsigned char result[inputLength];
    unsigned char length = 0;

    for (unsigned char i = 0; i < inputLength; i++, length++) {
        if ((input[i] & 0x80) && (i < inputLength - 1)) {
            result[length] = ((input[i] & 0x0F) << 4) | (input[i + 1]);
            i++;
        } else {
            result[length] = input[i];
        }
    }

    recipient = result[0];
    command = result[1];

    if (length > 2) {
        dataLength = length - 2;
        data = new unsigned char[dataLength];
        memcpy(data, &result[2], dataLength);
    } else {
        data = nullptr;
        dataLength = 0;
    }
}

InboundMessage::~InboundMessage() {
    delete[] data;
}

