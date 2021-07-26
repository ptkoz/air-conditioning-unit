#include <Arduino.h>
#include <Controller/PingController.h>
#include "Controller/RemoteCommandProcessor.h"
#include "Devices/DimplexPC35AMB.h"

ACC::Time::Source timeSource;
ACC::Devices::DimplexPC35AMB airConditioner(3, timeSource);
ACC::Controller::RemoteCommand::Processor remoteCommandProcessor(Serial, 2, airConditioner);
ACC::Controller::PingController pingController(timeSource, remoteCommandProcessor, 0xA1U, 0x90U);

void setup() {
    Serial.begin(9600);
    airConditioner.initialize();
    remoteCommandProcessor.initialize();
}

void loop() {
    remoteCommandProcessor.process();
    pingController.process();
}