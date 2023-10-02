#pragma once

#include "InboundProcessor.h"

namespace ACC::Security {
    class NounceProvider; // forward declaration
    class NounceRegistry; // forward declaration
}


namespace ACC::InboundProcessor {
    /**
     * A processor that is run on initialization that sets up the nounce for secure communication.
     */
    class InitializationProcessor : public InboundProcessor {
        private:
            Security::NounceProvider &nounceProvider;
            Security::NounceRegistry &nounceRegistry;

        public:
            explicit InitializationProcessor(
                Security::NounceProvider &nounceProvider,
                Security::NounceRegistry &nounceRegistry
            ) : nounceProvider(nounceProvider), nounceRegistry(nounceRegistry) {}

            /** Process incoming message */
            void process(const Radio::InboundMessage &message) override;
    };
}
