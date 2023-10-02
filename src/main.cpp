#ifndef PIO_UNIT_TESTING

#include "Arduino.h"
#include "Radio/Radio.h"
#include "Security/NounceProvider.h"
#include "Controller/PingController.h"
#include "InboundProcessor/OperationalProcessor.h"

#define MASTER_ADDRESS 0x10U
#define DIMPLEX_AC 0x30U
#define RELAY_HEATER 0x31U
#define VARIANT DIMPLEX_AC

ACC::Time::Source timeSource;
ACC::Security::NounceProvider nounceProvider;
ACC::Radio::Radio radio(Serial, 2, VARIANT, nounceProvider); // NOLINT(*-interfaces-global-init)

#if VARIANT == DIMPLEX_AC
    #include "Devices/DimplexPC35AMB.h"
    ACC::Devices::DimplexPC35AMB device(3, timeSource);
#elif VARIANT == RELAY_HEATER
    #include "Devices/RelayHeater.h"
    ACC::Devices::RelayHeater device;
#endif

ACC::Controller::PingController pingController(timeSource, radio, device, MASTER_ADDRESS, 0x01U);
ACC::InboundProcessor::OperationalProcessor operationalProcessor(device);

void setup() {
    // Disable internal led for power saving
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.begin(4800);
    device.initialize();
    radio.initialize();

    ACC::Security::NounceRegistry nounceRegistry;
    nounceProvider.initialize(timeSource, radio, nounceRegistry, MASTER_ADDRESS, 0x00U);
}

void loop() {
    pingController.process();
    radio.receive(operationalProcessor);
}

#endif // PIO_UNIT_TESTING