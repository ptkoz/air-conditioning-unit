#include "Arduino.h"
#include "BLAKE2s.h"
#include "Security/secrets.h"
#include "Radio/OutboundMessage.h"

using namespace ACC::Radio;

OutboundMessage::OutboundMessage(
    unsigned char from,
    unsigned char to,
    unsigned char command,
    unsigned long nounce,
    const void *extended,
    unsigned char extendedLength
) {
    if (extendedLength > 100) {
        // Having 255 byte limit for the whole message, we can't afford extended data longer than 100 bytes. Strip
        // the whole extended data to prevent buffer overflows.
        extendedLength = 0;
    }

    // 16 bytes for hmac + 4 bytes for nounce + 3 bytes for from, to & command + actual extended bytes
    unsigned char wholeMessageLength = 23 + extendedLength;

    unsigned char wholeMessage[wholeMessageLength];
    memcpy(&wholeMessage[16], &nounce, 4);
    wholeMessage[20] = from;
    wholeMessage[21] = to;
    wholeMessage[22] = command;
    memcpy(&wholeMessage[23], extended, extendedLength);

    // Put HMAC on first 16 bytes of the message
    BLAKE2s blake;
    blake.reset(ACC::Security::HMAC_KEY, ACC::Security::HMAC_KEY_LENGTH, 16);
    blake.update(&wholeMessage[16], wholeMessageLength - 16);
    blake.finalize(wholeMessage, 16);

    // 1 byte for message start marker + 1 byte for actual message length + worst case scenario for encoded bytes
    // (when each message byte is encoded on 2 chars to leave 2 highest bits 0)
    unsigned char maxEncodedBytesLength = 2 + 2 * wholeMessageLength;
    encodedBytes = new unsigned char[maxEncodedBytesLength];
    encodedBytes[0] = 0xFF; // every message starts with message start marker

    unsigned char addedBytes = 0;
    for (size_t i = 0; i < wholeMessageLength; i++) {
        if (wholeMessage[i] & 0x80) {
            // Highest bit is set, let's split this byte across two chars with only 4 low bits set on each, to ensure
            // start marker doesn't occur anywhere in the data stream. Then flag first byte with "10" on highest bits
            // which doesn't collide with start marker, but can inform consumer those need to be joined when reading.
            encodedBytes[2 + i + addedBytes] = (wholeMessage[i] >> 4) | 0x80;
            encodedBytes[2 + i + addedBytes + 1] = wholeMessage[i] & 0x0F;
            addedBytes++;
        } else {
            // highest bit is not set, we can leave value unencoded
            encodedBytes[2 + i + addedBytes] = wholeMessage[i];
        }
    }

    // original extended + added bytes
    encodedBytes[1] = wholeMessageLength + addedBytes;
}

OutboundMessage::~OutboundMessage() {
    delete[] encodedBytes;
}
