#pragma once

namespace ACC::Security {
    /**
     * Class that orchestrates storing initialization nounce in the EEPROM. Storing initialization nounce in the
     * registry ensures that the device is never initialized twice with the same starting nounce, which would allow
     * repetition attacks. It also ensures information is spread across whole EEPROM to preserve EEPROM save cycles.
     */
    class NounceRegistry {
        private:
            static constexpr unsigned short nounceSize = sizeof(unsigned long);
            /** What is our EEPROM block size that fit two nounce numbers? */
            static constexpr unsigned short blockSize = 2 * sizeof(unsigned long);
            /** What is the first EEPROM byte where we can save? */
            unsigned short startByte;
            /** How many EEPROM blocks we have available? */
            unsigned short numBlocks;
            /** Which block holds information about currently used nounce */
            unsigned short currentBlock;
            /** What was the last initialization nounce for inbound communication */
            unsigned long highestInboundNounce;
            /** What was the last initialization nounce for outbound communication */
            unsigned long highestOutboundNounce;
        public:
            /**
             * Initialize registry using whole available EEPROM memory.
             */
            NounceRegistry();

            /**
             * Initializes registry.
             *
             * @param startByte What is the first EEPROM byte where we can save?
             * @param numBytes How many bytes of EEPROM we can use?
             */
            NounceRegistry(int startByte, unsigned short numBytes);

            /**
             * Confirms that given inbound and outbound nounce can be used to initialize the device
             * because they have never been used before.
             */
            bool isNounceUnused(unsigned long inboundNounce, unsigned long outboundNounce) const;

            /**
             * Records that nounce initialization happened with given values.
             */
            void recordNounceInitialization(unsigned long inboundNounce, unsigned long outboundNounce);
    };
}
