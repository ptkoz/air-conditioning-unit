#include <Arduino.h>
#include "Controller/RemoteCommandRadio.h"

void ACC::Controller::RemoteCommand::Radio::initialize() {
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

void ACC::Controller::RemoteCommand::Radio::send(const ACC::Controller::RemoteCommand::OutboundMessage &message) {
    stream.write(message.getEncodedBytes(), message.getLength());
}
