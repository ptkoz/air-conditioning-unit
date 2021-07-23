#ifndef AIR_CONDITIONING_CONTROLLER_TIMESTAMP_H
#define AIR_CONDITIONING_CONTROLLER_TIMESTAMP_H

#include <Arduino.h>

namespace ACC::Time {
    /**
     * A class representing point in time.
     */
    class Timestamp {
        private:

        public:
            unsigned long millisSinceCpuStart;

            /** Construct with arbitrary time */
            inline explicit Timestamp(unsigned long millisSinceCpuStart):
                millisSinceCpuStart(millisSinceCpuStart) {}

            /** Construct with "current" time */
            inline explicit Timestamp():
                Timestamp(millis()) {}

            /** Copy constructor */
            inline Timestamp(const Timestamp & timestamp) = default;

            /** Assignment operator */
            inline Timestamp & operator=(const Timestamp & timestamp) = default;

            /**
             * Please bear in mind this is min age, not the precise age. This implementation cannot deliver precise
             * age due to 32bit integer overflow that happens roughly every 50 days. Is it guaranteed though that
             * the timestamp is older than returned value.
             *
             * The immediate consequence of the above is also that the age will never go beyond ~50days.
             */

            /** Gets timestamp min age in milliseconds */
            inline unsigned long getMinAgeMillis() const {
                return Timestamp().millisSinceCpuStart - millisSinceCpuStart;
            }

            /** Gets timestamp min age in seconds. Bear in mind precision is exactly same as for milliseconds */
            inline unsigned long getMinAgeSeconds() const {
                return getMinAgeMillis() / 1000;
            }
    };
}

#endif //AIR_CONDITIONING_CONTROLLER_TIMESTAMP_H
