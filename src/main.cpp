#ifndef PIO_UNIT_TESTING

#include <Arduino.h>
#include <Controller/PingController.h>
#include <Controller/RemoteCommandRadio.h>
#include "Controller/RemoteCommandProcessor.h"


#define DIMPLEX_AC 0x30U
#define RELAY_HEATER 0x31U
#define VARIANT DIMPLEX_AC

ACC::Time::Source timeSource;
ACC::Controller::RemoteCommand::Radio radio(Serial, 2);

#if VARIANT == DIMPLEX_AC
    #include "Devices/DimplexPC35AMB.h"
    ACC::Devices::DimplexPC35AMB device(3, timeSource);
#elif VARIANT == RELAY_HEATER
    #include "Devices/RelayHeater.h"
    ACC::Devices::RelayHeater device;
#endif

ACC::Controller::PingController pingController(timeSource, radio, device, VARIANT, 0x20U, 0x01U);
ACC::Controller::RemoteCommand::Processor remoteCommandProcessor(radio, device, VARIANT);

void setup() {
    // Disable internal led for power saving
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.begin(4800);
    device.initialize();
    radio.initialize();
}

void loop() {
    remoteCommandProcessor.process();
    pingController.process();
}

#endif // PIO_UNIT_TESTING