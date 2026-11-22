# Sender LoRa
This LoRa module is supposed to send information collected by the sensors to the receiving LoRa.

## Problems
* Our first issue was the soldering. The distance between the LoRa pins is smaller than the thickness of an average jumper wire, meaning that we had to do some finicky tricks.
* We wired the thing incorrectly to the breadboard 2 times.

## Wiring
* VIN - 3.3V
* GND - GND
* DIO0 - 0
* RESET - 1
* SCK - 4
* MISO - 5
* MOSI - 6
* NSS - 7

# Code
The code is integrated into the `UnifiedSenderScript.ino`
* Initialization is done in the `start` function with the lines `SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);` and `LoRa.setPins(LORA_NSS, LORA_RESET, LORA_DIO0);`.
* There are of course protections in case the LoRa is wired incorrectly, so that the ESP32 C3 supermini won't try to send stuff without a LoRa.
* Then, the code starts packets with `LoRa.beginPacket();` and then sends information over radio waves with `LoRa.println();`