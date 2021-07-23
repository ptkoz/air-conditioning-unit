#include <Arduino.h>
#include "Controller/WirelessController.h"
#include "Devices/DimplexPC35AMB.h"

ACC::Time::Source timeSource;
ACC::Devices::DimplexPC35AMB airConditioner(3, timeSource);
ACC::Controller::WirelessController controller(Serial, 2, airConditioner);

void setup() {
    Serial.begin(9600);
    airConditioner.initialize();
    controller.initialize();
}

void loop() {
    controller.process();
}