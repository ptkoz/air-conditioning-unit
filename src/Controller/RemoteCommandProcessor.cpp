#include "Controller/RemoteCommandProcessor.h"
#include "Controller/InboundMessage.h"

using ACC::Controller::RemoteCommand::Processor;

void Processor::process() {
    if (stream.available()) {

        stream.setTimeout(5000);
        unsigned char messageStartMark;
        if (stream.readBytes(&messageStartMark, 1) != 1) {
            // unable ro read byte, broken transmission?
            return;
        }

        if (messageStartMark != 0xFF) {
            // skip anything and wait for message beginning
            return;
        }

        unsigned char messageSize;
        if (stream.readBytes(&messageSize, 1) != 1) {
            // unable ro read byte, broken transmission?
            return;
        }

        stream.setTimeout(10000);
        unsigned char buffer[messageSize];
        if (stream.readBytes(buffer, messageSize) != messageSize) {
            // unable ro read message, broken transmission?
            return;
        }

        InboundMessage message(buffer, messageSize);
        if (message.getRecipient() != 0xA2 || !message.isValid(0)) {
            // the message isn't for us or is invalid, ignore it
            return;
        }

        switch (message.getCommand()) {
            case turnOnCommand: {
                airConditioner.turnOn();
                break;
            }
            case turnOffCommand: {
                airConditioner.turnOff();
                break;
            }
            case setLowSpeedCommand: {
                airConditioner.setLowSpeed();
                break;
            }
            case setHighSpeedCommand: {
                airConditioner.setHighSpeed();
                break;
            }
        }
    }
}
