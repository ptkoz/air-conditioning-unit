#include "Controller/PingController.h"
#include "Radio/OutboundMessage.h"

using namespace ACC::Controller;

void PingController::process() {
    if (lastPing.getMinAgeSeconds() > interval) {
        const unsigned char extendedData = controlledDevice.isWorking();
        publisher.send(
            sendToAddress,
            pingCommand,
            &extendedData,
            1
        );
        lastPing = timeSource.currentTimestamp();
    }
}
