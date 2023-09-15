#include "Controller/OutboundMessage.h"
#include "Arduino.h"

ACC::Controller::RemoteCommand::OutboundMessage::OutboundMessage(
    unsigned char address,
    unsigned char command,
    const void *message,
    unsigned char messageLength
) {
    if (messageLength > 120) {
        // Having 255 byte limit for the whole message, we can't afford unencoded messages longer than 120 bytes. Strip
        // the whole data to prevent buffer overflows.
        messageLength = 0;
    }

    // message has address, command and then remaining, optional part
    unsigned char wholeMessageLength = 2 + messageLength;
    // 2 times the message + message start byte + message length
    unsigned char maxEncodedMessageLength = 2 * wholeMessageLength + 2;

    // create buffer for encoded message
    encodedData = new unsigned char[maxEncodedMessageLength];
    // every message starts with message start marker
    encodedData[0] = 0xFF;

    unsigned char wholeMessage[wholeMessageLength];
    wholeMessage[0] = address;
    wholeMessage[1] = command;
    memcpy(&wholeMessage[2], message, messageLength);

    unsigned char addedBytes = 0;
    for (size_t i = 0; i < wholeMessageLength; i++) {
        if (wholeMessage[i] & 0x80) {
            // Highest bit is set, let's split this byte across two chars with only 4 low bits set on each, to ensure
            // start marker doesn't occur anywhere in the data stream. Then flag first byte with "10" on highest bits
            // which doesn't collide with start marker, but can inform consumer those need to be joined when reading.
            encodedData[2 + i + addedBytes] = (wholeMessage[i] >> 4) | 0x80;
            encodedData[2 + i + addedBytes + 1] = wholeMessage[i] & 0x0F;
            addedBytes++;
        } else {
            // highest bit is not set, we can leave value unencoded
            encodedData[2 + i + addedBytes] = wholeMessage[i];
        }
    }

    // original message + added bytes
    encodedData[1] = wholeMessageLength + addedBytes;
}

ACC::Controller::RemoteCommand::OutboundMessage::~OutboundMessage() {
    delete[] encodedData;
}
