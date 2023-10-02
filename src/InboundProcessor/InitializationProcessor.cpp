#include "InboundProcessor/InitializationProcessor.h"
#include "Security/NounceProvider.h"

using namespace ACC::InboundProcessor;

void InitializationProcessor::process(const Radio::InboundMessage &message) {
    if (message.getCommand() == 0x00 && message.getExtendedBytesLength() == sizeof(unsigned long)) {
        unsigned long outboundNounce;
        memcpy(&outboundNounce, message.getExtendedBytes(), sizeof(unsigned long));

        if (nounceRegistry.isNounceUnused(message.getNounce(), outboundNounce)) {
            nounceProvider.finalizeInitialization(message.getNounce(), outboundNounce);
            nounceRegistry.recordNounceInitialization(message.getNounce(), outboundNounce);
        }
    }
}