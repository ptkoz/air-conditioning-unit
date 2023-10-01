#include "Controller/OutboundMessage.h"
#include "Arduino.h"
#include "BLAKE2s.h"
#include "secrets.h"

ACC::Controller::RemoteCommand::OutboundMessage::OutboundMessage(
    unsigned char address,
    unsigned char command,
    unsigned long nounce,
    const void *message,
    unsigned char messageLength
) {
    if (messageLength > 100) {
        // Having 255 byte limit for the whole message, we can't afford unencoded messages longer than 100 bytes. Strip
        // the whole data to prevent buffer overflows.
        messageLength = 0;
    }

    // 16 bytes for hmac + 4 bytes for nounce + 1 byte for address + 1 byte for command + actual message bytes
    unsigned char wholeMessageLength = 22 + messageLength;
    // 1 byte for message start marker + 1 byte for remaining message length + worst case scenario for encoded bytes
    // (when each message bytes is encoded on 2 chars to leave 2 highest bits 0)
    unsigned char maxEncodedMessageLength = 2 + 2 * wholeMessageLength;

    // create buffer for encoded message
    encodedData = new unsigned char[maxEncodedMessageLength];
    // every message starts with message start marker
    encodedData[0] = 0xFF;

    unsigned char wholeMessage[wholeMessageLength];
    memcpy(&wholeMessage[16], &nounce, 4);
    wholeMessage[20] = address;
    wholeMessage[21] = command;
    memcpy(&wholeMessage[22], message, messageLength);

    BLAKE2s blake;
    blake.reset(ACC::Secrets::HMAC_KEY, ACC::Secrets::HMAC_KEY_LENGTH, 16);
    blake.update(&wholeMessage[16], wholeMessageLength - 16);
    blake.finalize(wholeMessage, 16);

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
