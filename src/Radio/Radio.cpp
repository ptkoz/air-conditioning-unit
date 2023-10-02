#include <Arduino.h>
#include "Radio/Radio.h"
#include "Radio/OutboundMessage.h"
#include "Security/NounceProvider.h"

using namespace ACC::Radio;

void Radio::initialize() {
    enterATMode();

    stream.println("AT+C003");
    delay(80);
    stream.println("AT+FU3");
    delay(80);
    stream.println("AT+P8");
    delay(80);
    stream.println("AT+B4800");
    delay(80);

    while (stream.available()) {
        stream.read();
    }

    leaveATMode();
}

void Radio::enterATMode() { // NOLINT(*-make-member-function-const)
    pinMode(setPin, OUTPUT);
    digitalWrite(setPin, LOW);
    delay(40);
}

void Radio::leaveATMode() { // NOLINT(*-make-member-function-const)
    pinMode(setPin, INPUT);
    digitalWrite(setPin, LOW);
    delay(80);
}

void Radio::send(
    unsigned char toAddress,
    unsigned char command,
    const void *extendedBytes,
    unsigned char extendedBytesLength
) {
    OutboundMessage message(
        ownAddress,
        toAddress,
        command,
        nounceProvider.nextOutboundNounce(),
        extendedBytes,
        extendedBytesLength
    );

    stream.write(message.getEncodedBytes(), message.getLength());
}

void Radio::receive(ACC::InboundProcessor::InboundProcessor &processor) {
    if (!stream.available()) {
        return;
    }

    stream.setTimeout(5000);
    unsigned char messageStartMark;
    if (stream.readBytes(&messageStartMark, 1) != 1) {
        // unable ro read byte, broken transmission?
        return;
    }

    if (messageStartMark != 0xFF) {
        // skip anything and wait for message beginning
        return;
    }

    unsigned char messageSize;
    if (stream.readBytes(&messageSize, 1) != 1) {
        // unable ro read byte, broken transmission?
        return;
    }

    stream.setTimeout(10000);
    unsigned char buffer[messageSize];
    if (stream.readBytes(buffer, messageSize) != messageSize) {
        // unable ro read message, broken transmission?
        return;
    }

    InboundMessage message(buffer, messageSize);
    if (message.getToAddress() == ownAddress && message.isValid(nounceProvider.getLastInboundNounce())) {
        nounceProvider.registerInboundNounce(message.getNounce());
        processor.process(message);
    }
}

