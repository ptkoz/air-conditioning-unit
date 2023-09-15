#include <Arduino.h>
#include "Controller/RemoteCommandRadio.h"

void ACC::Controller::RemoteCommand::Radio::initialize() {
    enterATMode();

    stream.println("AT+C003");
    delay(80);
    stream.println("AT+FU1");
    delay(80);
    stream.println("AT+B9600");
    delay(80);

    while (stream.available()) {
        stream.read();
    }

    leaveATMode();

    stream.setTimeout(1000);
}

void ACC::Controller::RemoteCommand::Radio::enterATMode() {
    pinMode(setPin, OUTPUT);
    digitalWrite(setPin, LOW);
    delay(40);
}

void ACC::Controller::RemoteCommand::Radio::leaveATMode() {
    pinMode(setPin, INPUT);
    digitalWrite(setPin, LOW);
    delay(80);
}

void ACC::Controller::RemoteCommand::Radio::encode(unsigned char *output, const unsigned char *data, size_t dataLength) const {
    for (size_t i = 0; i < dataLength; i++) {
        output[2 * i] = data[i] >> 4;
        output[2 * i + 1] = data[i] & 0x0F;
    }
}

void ACC::Controller::RemoteCommand::Radio::decode(unsigned char *output, const unsigned char *data, size_t outputLength) const {
    for (size_t i = 0; i < outputLength; i++) {
        output[i] = (data[2 * i] << 4) | data[2 * i + 1];
    }
}
