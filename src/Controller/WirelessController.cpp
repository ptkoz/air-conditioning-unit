#include <Arduino.h>
#include "Controller/WirelessController.h"

using ACC::Controller::WirelessController;

void WirelessController::initialize() {
    pinMode(setPin, OUTPUT);
    digitalWrite(setPin, LOW);

    dataStream.print("AT+C003");
    while (dataStream.read() != -1);

    dataStream.print("AT+FU1");
    while (dataStream.read() != -1);

    dataStream.print("AT+B9600");
    while (dataStream.read() != -1);

    pinMode(setPin, INPUT);
    digitalWrite(setPin, LOW);

    dataStream.setTimeout(1000);
}

void ACC::Controller::WirelessController::process() {
    if (dataStream.available()) {
        short int address;
        if (dataStream.readBytes(static_cast<char *>(static_cast<void *>(&address)), sizeof address) !=
            sizeof address) {
            // unable ro read address, broken transmission?
            return;
        }

        if (address != 0xA2) {
            // advance to the end of the message
            dataStream.find((unsigned char) 0);
            return;
        }

        short int command;
        if (dataStream.readBytes(static_cast<char *>(static_cast<void *>(&command)), sizeof address) !=
            sizeof address) {
            // unable to read command, broken transmission?
            return;
        }

        switch (command) {
            case 0x01:
                airConditioner.turnOn();
                dataStream.find((unsigned char) 0);
                break;
            case 0x02:
                airConditioner.turnOff();
                dataStream.find((unsigned char) 0);
                break;
        }
    }
}