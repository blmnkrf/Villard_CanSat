# ESP32 C3 Super Mini
<img src="./Images/ESP32 C3 Super Mini.png" alt="ESP32 C3 Super Mini" width="200px"/>

The ESP32 C3 Super Mini is the microcontroller we are using for our CanSat project. 

# Problems
The second we got our hands on it and wanted to start working with it, we encountered a MULTITUDE of problems.
* We realized that we had to solder its legs. This was something we wanted to avoid at first, since we wanted to find a way to not solder stuff until we had everythnig working. In the end, we realized that it would simply be impossible and we had to solder its legs, though we would use a breadboard for now.
* After soldering, we tried connecting it to Cristi's laptop. The issue was simple: the Arduino IDE didn't recognize it. We spent about 2 hours trying to fix it, and it was because specifically the ESP32 C3 Super Mini lacks the pull down resistor. Basically, this means you can't connect it to a laptop with a USB C to C, but you'd need a USB A to C, since USB C to C uses 2 resistors to authentificate a connection, and this specific ESP lacked one of them. We ended up using Mr.Rodas' cable, since it seems like in Moldova all USB A to C cables one day just went extinct and vanished.
* We had soldered the BME280 and had it connected directly to our ESP. Cristi's laptop couldn't detect it again. What was the issue? The issue was the BME280, for some reason. The ESP would simply get extremely hot and still not connect to the laptop. All we had to do was unplug the BME280 from the ESP32. Then the laptop returned to its senses and recognized the ESP32.