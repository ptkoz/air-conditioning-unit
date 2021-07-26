#include <Arduino.h>
#include "Controller/RemoteCommandProcessor.h"

using ACC::Controller::RemoteCommand::Processor;

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
