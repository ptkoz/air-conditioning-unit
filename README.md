# Air Conditioning Unit

A device that listens for commands from [Home Climate Controller](https://github.com/ptkoz/infodisplay-controller) on
Serial port (with HC-12 433 Mhz radio bridge) and then sends IR signals to the actual Air Conditioner, pretending to be
its remote control.

Also, periodically sends pings to the [Home Climate Controller](https://github.com/ptkoz/infodisplay-controller) to make
it aware it's alive.