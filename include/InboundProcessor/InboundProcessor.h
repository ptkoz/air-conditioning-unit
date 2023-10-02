#pragma once

#include <Stream.h>
#include "Radio/InboundMessage.h"

namespace ACC::InboundProcessor {
    /**
     * Processor for incoming messages, determines what action (if any) needs to be taken
     * when a message is received.
     */
    class InboundProcessor {
        public:
            /** Process incoming message */
            virtual void process(const Radio::InboundMessage &message) = 0;
    };
}
