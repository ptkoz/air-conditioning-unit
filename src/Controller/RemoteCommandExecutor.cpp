#include "Controller/RemoteCommandExecutor.h"

void ACC::Controller::RemoteCommand::Executor::execute(
    unsigned short address,
    unsigned short command,
    const void * message,
    size_t messageLength
) {
    size_t shortLength = sizeof(unsigned short);
    unsigned char headerBuffer[2 * shortLength];

    radio.encode(headerBuffer, static_cast<const unsigned char *>(static_cast<const void *>(&address)), shortLength);
    stream.write(headerBuffer, 2 * shortLength);

    radio.encode(headerBuffer, static_cast<const unsigned char *>(static_cast<const void *>(&command)), shortLength);
    stream.write(headerBuffer, 2 * shortLength);

    unsigned char messageBuffer[2 * messageLength];
    radio.encode(messageBuffer, static_cast<const unsigned char *>(message), messageLength);
    stream.write(messageBuffer, 2 * messageLength);
    stream.write((char) 0xFF);
}
