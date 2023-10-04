#include "Devices/RelayHeater.h"
#include "Arduino.h"

ACC::Devices::RelayHeater::RelayHeater(unsigned char relayPin): relayPin(relayPin), isTurnedOn(false) {
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW);
}

bool ACC::Devices::RelayHeater::turnOn() {
    digitalWrite(relayPin, HIGH);
    isTurnedOn = true;
    return false;
}

bool ACC::Devices::RelayHeater::turnOff() {
    digitalWrite(relayPin, LOW);
    isTurnedOn = false;
    return false;
}
