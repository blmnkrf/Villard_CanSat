# Receiving LoRa
This LoRa must listen for packages from the sending LoRa, parse them, and then use the signal strength to approximate the distance between the two LoRa modules.

## Problems
* Unlike with the sender LoRa, there were no issues with this, excluding the one time we did the wiring wrong. We tried excluding the part of the code that checks if the connection is correct, and it started spewing nonsense.

## Wiring
* VIN - 3.3V
* GND - GND
* DIO0 - 0
* RESET - 1
* SCK - 4
* MISO - 5
* MOSI - 6
* NSS - 7

## Code
The code can be found in the `LoRa_Receiver.ino` script.
* Initializaition is done in the `start` function, in the lines `SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);` and `LoRa.setPins(LORA_NSS, LORA_RESET, LORA_DIO0);`.
* There are also checks for the connection. If the LoRa isn't connected correctly, the code will simply not run, since else you will receive nonsense as information (we tried).
* In the `loop` function, the LoRa listens for packages. Thankfully, this whole process is easy, thanks to the LoRa library we are using.
* You get the signal strength with `LoRa.packetRssi();`. Afterwards, we use this line of code to approximate the distance `pow(10, ((ONE_METER_RSSI - (signalStrength)) / (10 * ENVIRONMENTAL_CONSTANT)));`.
    * Full formula: $10^{\frac{A - RSSI}{10*N}}$
    * $A$ - $RSSI$ (signal strength) between the 2 LoRa modules at 1 meter distance. You must perform measurements by yourself for this, and you can't just attempt it "calculate it" or get somebody else's numbers from the internet
    * $N$ - The environmental constant. This is a constant that you must choose based on the environment the LoRa modules are in. For example, it is 2 when you're on an empty field, and 4 when you're in a building.
    * This formula works using the inverse square law. $RSSI \sim \frac{1}{r^2}$, where $RSSI$ is signal strength and $r$ is the distance between the radii. This works since LoRa modules communicate using radio waves, which obey the inverse square law.
    * Unfortunately, we live in real life and this formula is at best an approximation, though it is good enough. This is what $N$ is for. If you put it as 2, it will end up like $10^{\frac{A - RSSI}{20}}$, meaning that we will take the 20th root of 10, which in this case is the inverse square law. If $N$ is for example 3, you will take the 30th root.