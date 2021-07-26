#ifndef AIR_CONDITIONING_RECEIVER_REMOTECOMMANDEXECUTOR_H
#define AIR_CONDITIONING_RECEIVER_REMOTECOMMANDEXECUTOR_H

#include "Arduino.h"
#include "RemoteCommandRadio.h"

namespace ACC::Controller::RemoteCommand {
    /**
     * Interface for executing remote commands
     */
    class Executor {
        private:
            Stream & stream;
        public:
            explicit Executor(const Radio & radio):
                stream(radio.getStream()) {}

            void execute(
                unsigned short address,
                unsigned short command,
                const void * message = nullptr,
                size_t length = 0
            );
    };
}

#endif //AIR_CONDITIONING_RECEIVER_REMOTECOMMANDEXECUTOR_H
