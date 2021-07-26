#include <Arduino.h>
#include <Controller/PingController.h>
#include <Controller/RemoteCommandRadio.h>
#include "Controller/RemoteCommandProcessor.h"
#include "Devices/DimplexPC35AMB.h"

ACC::Time::Source timeSource;
ACC::Devices::DimplexPC35AMB airConditioner(3, timeSource);

ACC::Controller::RemoteCommand::Radio radio(Serial, 2);
ACC::Controller::RemoteCommand::Processor remoteCommandProcessor(radio, airConditioner);
ACC::Controller::RemoteCommand::Executor remoteCommandExecutor(radio);
ACC::Controller::PingController pingController(timeSource, remoteCommandExecutor, 0xA1U, 0x90U);

void setup() {
    Serial.begin(9600);
    airConditioner.initialize();
    radio.initialize();
}

void loop() {
    remoteCommandProcessor.process();
    pingController.process();
}