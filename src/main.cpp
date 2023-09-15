#include <Arduino.h>
#include <Controller/PingController.h>
#include <Controller/RemoteCommandRadio.h>
#include "Controller/RemoteCommandProcessor.h"
#include "Devices/DimplexPC35AMB.h"

ACC::Time::Source timeSource;
ACC::Devices::DimplexPC35AMB airConditioner(3, timeSource);

ACC::Controller::RemoteCommand::Radio radio(Serial, 2);
ACC::Controller::RemoteCommand::Processor remoteCommandProcessor(radio, airConditioner);
ACC::Controller::PingController pingController(timeSource, radio, 0xA1U, 0x90U);

void setup() {
    // Disable internal led for power saving
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.begin(4800);
    airConditioner.initialize();
    radio.initialize();
}

void loop() {
    remoteCommandProcessor.process();
    pingController.process();
}