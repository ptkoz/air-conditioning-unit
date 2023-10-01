#include "Controller/PingController.h"
#include "Controller/OutboundMessage.h"

void ACC::Controller::PingController::process() {
    if (lastPing.getMinAgeSeconds() > interval) {
        const unsigned char extendedData = controlledDevice.isWorking();
        radio.send(
            ACC::Controller::RemoteCommand::OutboundMessage(
                fromAddress,
                toAddress,
                pingCommand,
                0,
                &extendedData,
                1
            )
        );
        lastPing = timeSource.currentTimestamp();
    }
}
