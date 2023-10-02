#include "Arduino.h"
#include "BLAKE2s.h"
#include "Radio/InboundMessage.h"
#include "Security/secrets.h"

using namespace ACC::Radio;

InboundMessage::InboundMessage(const unsigned char *input, unsigned char inputLength)
    : fromAddress(0), toAddress(0), command(0), nounce(0), isHmacValid(false), extendedBytes(nullptr), extendedBytesLength(0) {

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

    if (length < 23) {
        return;
    }

    BLAKE2s blake;
    blake.reset(ACC::Security::HMAC_KEY, ACC::Security::HMAC_KEY_LENGTH, 16);
    blake.update(&result[16], length - 16);

    unsigned char hmac[16];
    blake.finalize(hmac, 16);

    isHmacValid = (memcmp(hmac, result, 16) == 0);
    memcpy(&nounce, &result[16], 4);

    fromAddress = result[20];
    toAddress = result[21];
    command = result[22];

    if (length > 23) {
        extendedBytesLength = length - 23;
        extendedBytes = new unsigned char[extendedBytesLength];
        memcpy(extendedBytes, &result[23], extendedBytesLength);
    }
}

InboundMessage::~InboundMessage() {
    delete[] extendedBytes;
}

bool InboundMessage::isValid(unsigned long lastInboundNounce) const {
    return isHmacValid && nounce > lastInboundNounce;
}

