#ifndef AIR_CONDITIONING_UNIT_SOURCE_H
#define AIR_CONDITIONING_UNIT_SOURCE_H

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

#endif //AIR_CONDITIONING_UNIT_SOURCE_H
