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
        EEPROM.get((int) (i * blockSize + startByte), inbound);
        EEPROM.get((int) (i * blockSize + startByte + nounceSize), outbound);

        if (highestInboundNounce <= inbound && highestOutboundNounce <= outbound) {
            highestInboundNounce = inbound;
            highestOutboundNounce = outbound;
            currentBlock = i;
        }
    }
}

bool NounceRegistry::isNounceUnused(unsigned long inboundNounce, unsigned long outboundNounce) const {
    return inboundNounce > highestInboundNounce && outboundNounce >= highestOutboundNounce;
}

void NounceRegistry::recordNounceInitialization( // NOLINT(*-make-member-function-const)
    unsigned long inboundNounce,
    unsigned long outboundNounce
) {
    unsigned short nextBlock = (currentBlock + 1) % numBlocks;
    EEPROM.put((int) (nextBlock * blockSize + startByte), inboundNounce);
    EEPROM.put((int) (nextBlock * blockSize + startByte + nounceSize), outboundNounce);
}

void NounceRegistry::resetMemory() { // NOLINT(*-make-member-function-const)
    unsigned long zero = 0;
    for (unsigned short i = 0; i < numBlocks; i++) {
        EEPROM.put((int) (i * blockSize + startByte), zero);
        EEPROM.put((int) (i * blockSize + startByte + nounceSize), zero);
    }
}
