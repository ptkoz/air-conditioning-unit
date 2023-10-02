#include "InboundProcessor/OperationalProcessor.h"
#include "Radio/InboundMessage.h"

using namespace ACC::InboundProcessor;

void OperationalProcessor::process(const Radio::InboundMessage &message) {
    switch (message.getCommand()) {
        case turnOnCommand: {
            controlledDevice.turnOn();
            break;
        }
        case turnOffCommand: {
            controlledDevice.turnOff();
            break;
        }
    }
}
