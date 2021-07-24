#include <Arduino.h>
#include "Controller/RemoteCommandProcessor.h"

using ACC::Controller::RemoteCommandProcessor;

void RemoteCommandProcessor::initialize() {
    pinMode(setPin, OUTPUT);
    digitalWrite(setPin, LOW);

    stream.print("AT+C003");
    while (stream.read() != -1);

    stream.print("AT+FU1");
    while (stream.read() != -1);

    stream.print("AT+B9600");
    while (stream.read() != -1);

    pinMode(setPin, INPUT);
    digitalWrite(setPin, LOW);

    stream.setTimeout(1000);
}

void ACC::Controller::RemoteCommandProcessor::process() {
    if (stream.available()) {
        unsigned short address;
        if (stream.readBytes(static_cast<char *>(static_cast<void *>(&address)), sizeof address) != sizeof address) {
            // unable ro read address, broken transmission?
            return;
        }

        if (address != listenAddress) {
            // advance to the end of the message
            stream.find((unsigned char) 0);
            return;
        }

        unsigned short command;
        if (stream.readBytes(static_cast<char *>(static_cast<void *>(&command)), sizeof command) != sizeof command) {
            // unable to read command, broken transmission?
            return;
        }

        switch (command) {
            case turnOnCommand: {
                airConditioner.turnOn();
                stream.find((unsigned char) 0);
                break;
            }
            case turnOffCommand: {
                airConditioner.turnOff();
                stream.find((unsigned char) 0);
                break;
            }
            default: {
                stream.find((unsigned char) 0);
                break;
            }
        }
    }
}