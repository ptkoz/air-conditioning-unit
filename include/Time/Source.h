#pragma once

#include "Timestamp.h"

namespace ACC::Time {
    /**
     * A time source capable of providing timestamps.
     */
    class Source {
        public:
            inline Timestamp currentTimestamp() const {
                return Timestamp();
            }
    };
}
