# IoTrains

Internet of Trains :)


The existing systems in the market are either with old roots (DCC) or not opensource (Märklin). More then that they were built over 20 years ago and therefore have historical restrictions. In order to be able more flexible I started to write a firmware for the trains and accessories using ESP32/ESP8266.

The projects consists of:
 - [Firmware](./firmware/) which is written in c++ using PlatformIO,
 - MQTT Broker, see [emqx](./emqx) as an example,
 - A [backend](./webui/server) written in typesript,
 - as well [frontend](./webui/client/) in react,
 - [json schema](./jsonschema/) describes a mqtt based protocol.

# Notes
 - Connect an audio amplifier to an i2s capable pin i.e. DAC_1.

## TODO
 - docker for backend
 - responsive ui / refresh on update
 - process rfid events
 - sqlite3
 - schematic
