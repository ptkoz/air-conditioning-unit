#pragma once

#include "Stream.h"
#include "Publisher.h"
#include "InboundProcessor/InboundProcessor.h"

namespace ACC::Security {
    class NounceProvider; // forward declaration
}

namespace ACC::Radio {
    /**
     * Radio abstraction layer for processor & publisher
     */
    class Radio : public Publisher {
        private:
            Stream &stream;
            Security::NounceProvider &nounceProvider;
            const unsigned char setPin;
            const unsigned char ownAddress;

            /** Enters HC-12 module into configuration mode */
            void enterATMode();

            /** Exits HC-12 configuration mode */
            void leaveATMode();

        public:
            Radio(
                Stream &stream,
                unsigned char setPin,
                unsigned char ownAddress,
                Security::NounceProvider &nounceProvider
            ) :
                stream(stream),
                nounceProvider(nounceProvider),
                setPin(setPin),
                ownAddress(ownAddress) {}

            void initialize();

            void send(
                unsigned char toAddress,
                unsigned char command,
                const void *extendedBytes,
                unsigned char extendedBytesLength
            ) override;

            using Publisher::send;

            void receive(InboundProcessor::InboundProcessor &processor);
    };
}
