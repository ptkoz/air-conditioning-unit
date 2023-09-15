#include <Arduino.h>
#include "Controller/RemoteCommandProcessor.h"

using ACC::Controller::RemoteCommand::Processor;

void Processor::process() {
    size_t shortLength = sizeof(unsigned short);
    unsigned char buffer[2 * shortLength];

    if (stream.available()) {
        if (stream.readBytes(buffer, 2 * shortLength) != 2 * shortLength) {
            // unable ro read address, broken transmission?
            return;
        }

        unsigned short address;
        radio.decode(static_cast<unsigned char *>(static_cast<void *>(&address)), buffer, shortLength);

        if (address != listenAddress) {
            // advance to the end of the message
            stream.find((char) 0xFF);
            return;
        }

        if (stream.readBytes(buffer, 2 * shortLength) != 2 * shortLength) {
            // unable to read command, broken transmission?
            return;
        }

        unsigned short command;
        radio.decode(static_cast<unsigned char *>(static_cast<void *>(&command)), buffer, shortLength);

        switch (command) {
            case turnOnCommand: {
                airConditioner.turnOn();
                stream.find((char) 0xFF);
                break;
            }
            case turnOffCommand: {
                airConditioner.turnOff();
                stream.find((char) 0xFF);
                break;
            }
            case setLowSpeedCommand: {
                airConditioner.setLowSpeed();
                stream.find((char) 0xFF);
                break;
            }
            case setHighSpeedCommand: {
                airConditioner.setHighSpeed();
                stream.find((char) 0xFF);
                break;
            }
            default: {
                stream.find((char) 0xFF);
                break;
            }
        }
    }
}
