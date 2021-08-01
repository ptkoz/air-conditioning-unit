# Air Conditioning Unit

A device that listens for commands on Serial port (with HC-12 433 Mhz radio bridge) and then sends IR signals to the
actual Air Conditioner, pretending to be its remote control.

Also periodically sends pings to
the [Air Conditioning Controller](https://github.com/pamelus/air-conditioning-controller) to make it aware it's alive.