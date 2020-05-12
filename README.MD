# X10 to ESP32 MQTT HomeAssistant 

[![Build Status](https://travis-ci.org/aleksey2000/esp32-x10-mqtt-ha.svg?branch=master)](https://travis-ci.org/github/aleksey2000/esp32-x10-mqtt-ha)

Small firmware for esp32. Provides integration between x10 dimmers and Home Assistant using mqtt.

# Setup

Before start, you have to setup at leat following options:


# Hardware config

| Parameter | Description |
| ------ | ------ |
| zcPin | the zero crossing detect pin |
| dataPin | the TX X10 data out pin |
| dataRecievePin | the TX X10 data out pin |
| repeatTimes | how many times each X10 message should repeat |

# Network config

| Parameter | Description |
| ------ | ------ |
| ssid[] | wifi ssid |
| pass[] | wifi pass |
| local_IP | static ip for your esp32 |
| gateway | gateway ip |
| subnet | subnet mask for your network|
| primaryDNS | DNS |
| secondaryDNS | DNS |

# Home assisatnt config

| Parameter | Description |
| ------ | ------ |
| IPAddress server(192,168,x,x); | HOME ASSISTANT IP |
| ha_ip | home assistant ip (again, but in char[]) |
| ha_port | change it if you are not using default (8123) |
| ha_pwd | long-lived access token for HA access |



