#pragma once

namespace ACC::Radio {
    /**
     * Interface for publishing outbound messages
     */
    class Publisher {
        public:
            /**
             * Publish given message
             */
            virtual void send(
                unsigned char to,
                unsigned char command,
                const void *extendedBytes,
                unsigned char extendedBytesLength
            ) = 0;

            void send(unsigned char to, unsigned char command) {
                send(to, command, nullptr, 0);
            };
    };
}
