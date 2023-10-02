#pragma once

#include "NounceRegistry.h"
#include "Time/Source.h"

namespace ACC::Radio {
    class Radio; // forward declaration
}

namespace ACC::Security {
    /**
     * Provides nounce for over the publisher communication.
     */
    class NounceProvider {
        private:
            bool isInitialized;
            unsigned long lastInboundNounce;
            unsigned long lastOutboundNounce;
        public:
            NounceProvider()
                : isInitialized(false),
                  lastInboundNounce(0),
                  lastOutboundNounce(0) {}

            void initialize(
                const Time::Source &timeSource,
                Radio::Radio &radio,
                NounceRegistry &nounceRegistry,
                unsigned char sendRequestTo,
                unsigned char requestCommand
            );

            unsigned long nextOutboundNounce() {
                return isInitialized ? ++lastOutboundNounce : 0;
            };

            unsigned long getLastInboundNounce() const {
                return isInitialized ? lastInboundNounce : 0;
            };

            void registerInboundNounce(unsigned long nounce) {
                if (isInitialized) lastInboundNounce = nounce;
            };

            void finalizeInitialization(unsigned long inboundNounce, unsigned long outboundNounce);
    };
}
