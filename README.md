<div align="center">

<img src="https://github.com/speeduino/wiki.js/raw/master/img/Speeduino%20logo_med.png" alt="Speeduino" width="600" />

[![License](https://img.shields.io/badge/license-GPLv3-blue.svg)](https://github.com/noisymime/speeduino/blob/master/LICENSE)
[![Sponsors](https://img.shields.io/github/sponsors/noisymime)](https://github.com/sponsors/noisymime)
[![https://img.shields.io/discord/879495735912071269 ](https://img.shields.io/discord/879495735912071269?label=Discord&logo=Discord)](https://discord.gg/YWCEexaNDe)

##### A low cost, DIY friendly Engine Management System (ECU) based on the Arduino framework
</div>


## Speeduino Airbear
Airbear is an add-on board for Speeduino based ECUs based on the ESP32-C3 chip.It is designed to add a bluetooth and wifi interface to Speeduino ECUs, including a web based dash. 

It is currently in VERY early stages of development and is not yet ready for use.

## Documentation
In progress

## Hardware
Any ESP32-C3 board will work, however the intended target board is the low cost ESP32-C3 Supermini 
This can be found on many retailers including AliExpress and eBay. 

## Usage
Airbear consists of 2 main components, the firmware on the board itself and the web based dashboard.

The firmware should be installed via PlatformIO:

1. Open the project folder in PlatformIO
2. Go to the PlatformIO tab on the left hand side
3. From within the 'Project Tasks' menu, select 'Upload'
4. From within the same menu, select 'Upload Filesystem Image'

Once the firmware is loaded on the board, configuration can be performed via the web interface. Initially the board will create a wifi AP with the SSID 'Speeduino Dash' and no password. Connect to this wifi network and then open a browser to [http://speeduino.local/config](http://speeduino.local/config) in order to configure the unit. 