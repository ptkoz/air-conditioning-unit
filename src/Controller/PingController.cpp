#include "Controller/PingController.h"
#include "Controller/OutboundMessage.h"

void ACC::Controller::PingController::process() {
    if (lastPing.getMinAgeSeconds() > interval) {
        radio.send(ACC::Controller::RemoteCommand::OutboundMessage(recipientAddress, recipientCommand, 0));
        lastPing = timeSource.currentTimestamp();
    }
}
