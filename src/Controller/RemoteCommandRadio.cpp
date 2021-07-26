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
        stream.write(stream.read());
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
