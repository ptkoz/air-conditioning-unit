#include "Security/NounceProvider.h"
#include "InboundProcessor/InitializationProcessor.h"
#include "Radio/Radio.h"

using namespace ACC::Security;

void NounceProvider::initialize(
    const Time::Source &timeSource,
    Radio::Radio &radio,
    NounceRegistry &nounceRegistry,
    unsigned char sendRequestTo,
    unsigned char requestCommand
) {
    InboundProcessor::InitializationProcessor initializationProcessor(*this, nounceRegistry);
    while (!isInitialized) {
        radio.send(sendRequestTo, requestCommand);
        Time::Timestamp receivingStartTimestamp = timeSource.currentTimestamp();
        while (!isInitialized && receivingStartTimestamp.getMinAgeSeconds() > 30) {
            radio.receive(initializationProcessor);
        }
    }
}

void NounceProvider::finalizeInitialization(unsigned long inboundNounce, unsigned long outboundNounce) {

        isInitialized = true;
        lastInboundNounce = inboundNounce;
        lastOutboundNounce = outboundNounce;
}
