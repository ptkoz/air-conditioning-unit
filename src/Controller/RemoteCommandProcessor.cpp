#include <Arduino.h>
#include "Controller/RemoteCommandProcessor.h"

using ACC::Controller::RemoteCommandProcessor;

void RemoteCommandProcessor::initialize() {
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

void ACC::Controller::RemoteCommandProcessor::process() {
    if (dataStream.available()) {
        unsigned short address;
        if (dataStream.readBytes(static_cast<char *>(static_cast<void *>(&address)), sizeof address) !=
            sizeof address) {
            // unable ro read address, broken transmission?
            return;
        }

        if (address != listenAddress) {
            // advance to the end of the message
            dataStream.find((unsigned char) 0);
            return;
        }

        unsigned short command;
        if (dataStream.readBytes(static_cast<char *>(static_cast<void *>(&command)), sizeof address) !=
            sizeof address) {
            // unable to read command, broken transmission?
            return;
        }

        switch (command) {
            case turnOnCommand:
                airConditioner.turnOn();
                dataStream.find((unsigned char) 0);
                break;
            case turnOffCommand:
                airConditioner.turnOff();
                dataStream.find((unsigned char) 0);
                break;
            default:
                dataStream.find((unsigned char) 0);
                break;
        }
    }
}