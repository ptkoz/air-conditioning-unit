#include "Controller/PingController.h"

void ACC::Controller::PingController::process() {
    if (lastPing.getMinAgeSeconds() > interval) {
        executor.execute(recipientAddress, recipientCommand);
        lastPing = timeSource.currentTimestamp();
    }
}
