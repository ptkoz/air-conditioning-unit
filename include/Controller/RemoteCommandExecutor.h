#ifndef AIR_CONDITIONING_RECEIVER_REMOTECOMMANDEXECUTOR_H
#define AIR_CONDITIONING_RECEIVER_REMOTECOMMANDEXECUTOR_H

#include "Arduino.h"

namespace ACC::Controller::RemoteCommand {
    /**
     * Interface for executing remote commands
     */
     class Executor {
         public:
             virtual void execute(
                 unsigned short address,
                 unsigned short command,
                 const void * message = nullptr,
                 size_t length = 0
             ) = 0;
     };
}

#endif //AIR_CONDITIONING_RECEIVER_REMOTECOMMANDEXECUTOR_H
