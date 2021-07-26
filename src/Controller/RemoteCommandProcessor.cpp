#include <Arduino.h>
#include "Controller/RemoteCommandProcessor.h"

using ACC::Controller::RemoteCommand::Processor;

void Processor::initialize() {
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

void Processor::enterATMode() {
    pinMode(setPin, OUTPUT);
    digitalWrite(setPin, LOW);
    delay(40);
}

void Processor::leaveATMode() {
    pinMode(setPin, INPUT);
    digitalWrite(setPin, LOW);
    delay(80);
}

void Processor::process() {
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
            case setLowSpeedCommand: {
                airConditioner.setLowSpeed();
                stream.find((unsigned char) 0);
                break;
            }
            case setHighSpeedCommand: {
                airConditioner.setHighSpeed();
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

void Processor::execute(
    unsigned short address,
    unsigned short command,
    const void * message,
    size_t length
) {
    stream.write(static_cast<const char *>(static_cast<const void *>(&address)), sizeof address);
    stream.write(static_cast<const char *>(static_cast<const void *>(&command)), sizeof command);
    stream.write(static_cast<const char *>(message), length);
    stream.write((char) 0);
}