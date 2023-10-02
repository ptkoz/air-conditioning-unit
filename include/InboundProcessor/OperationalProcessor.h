#pragma once

#include "Devices/Device.h"
#include "InboundProcessor.h"


namespace ACC::InboundProcessor {
    /**
     * Wireless controller reads remote commands and reacts to them
     */
    class OperationalProcessor : public InboundProcessor {
        private:
            Devices::Device &controlledDevice;

            /** Recognized command codes */
            static constexpr unsigned char turnOnCommand = 0x01U;
            static constexpr unsigned char turnOffCommand = 0x02U;
        public:
            explicit OperationalProcessor(Devices::Device &controlledDevice)
                : controlledDevice(controlledDevice) {}

            /** Process incoming message */
            void process(const Radio::InboundMessage &message) override;
    };
}
