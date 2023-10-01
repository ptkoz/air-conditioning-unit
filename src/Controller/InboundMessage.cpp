#include "Controller/InboundMessage.h"
#include "Arduino.h"
#include "BLAKE2s.h"
#include "secrets.h"

using namespace ACC::Controller::RemoteCommand;

InboundMessage::InboundMessage(const unsigned char *input, unsigned char inputLength)
    : recipient(0), command(0), nounce(0), isHmacValid(false), data(nullptr), dataLength(0) {

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

    if (length < 22) {
        return;
    }

    BLAKE2s blake;
    blake.reset(ACC::Secrets::HMAC_KEY, ACC::Secrets::HMAC_KEY_LENGTH, 16);
    blake.update(&result[16], length - 16);

    unsigned char hmac[16];
    blake.finalize(hmac, 16);

    isHmacValid = (memcmp(hmac, result, 16) == 0);
    memcpy(&nounce, &result[16], 4);

    recipient = result[20];
    command = result[21];

    if (length > 22) {
        dataLength = length - 22;
        data = new unsigned char[dataLength];
        memcpy(data, &result[22], dataLength);
    }
}

InboundMessage::~InboundMessage() {
    delete[] data;
}

bool InboundMessage::isValid(unsigned long lastInputNounce) {
    return isHmacValid && nounce > lastInputNounce;
}

