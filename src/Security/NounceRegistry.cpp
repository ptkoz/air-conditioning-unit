#include "Security/NounceRegistry.h"
#include "EEPROM.h"

using namespace ACC::Security;

NounceRegistry::NounceRegistry() : NounceRegistry(0, EEPROM.length()) {}

NounceRegistry::NounceRegistry(int startByte, unsigned short numBytes)
    : startByte(startByte),
      numBlocks(numBytes / blockSize),
      currentBlock(0),
      highestInboundNounce(0),
      highestOutboundNounce(0) {

    for (unsigned short i = 0; i < numBlocks; i++) {
        unsigned long inbound, outbound;
        EEPROM.get((int)(numBlocks * blockSize + startByte), inbound);
        EEPROM.get((int)(numBlocks * blockSize + startByte + nounceSize), outbound);

        if (highestOutboundNounce < inbound && highestOutboundNounce < outbound) {
            highestInboundNounce = inbound;
            highestOutboundNounce = outbound;
            currentBlock = i;
        }
    }
}

bool NounceRegistry::isNounceUnused(unsigned long inboundNounce, unsigned long outboundNounce) const {
    return inboundNounce > highestInboundNounce && outboundNounce >= highestOutboundNounce;
}

void NounceRegistry::recordNounceInitialization(unsigned long inboundNounce, unsigned long outboundNounce) {
    unsigned short nextBlock = (currentBlock + 1) % numBlocks;
    EEPROM.put((int)(nextBlock * blockSize + startByte), inboundNounce);
    EEPROM.put((int)(nextBlock * blockSize + startByte + nounceSize), outboundNounce);
}
