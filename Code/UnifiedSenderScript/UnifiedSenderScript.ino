// Importing the libraries needed for the BME280, including the Wire protocol
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

// Importing the libraries needed for the LoRa module, including the SPI protocol
#include <SPI.h>
#include <LoRa.h>

Adafruit_BME280 bme; // The BME280 object

// I'm using constexpr instead of #define since it is actually compile time and saves RAM

// The ESP pins needed for the BME280
constexpr int I2C_SDA = 3;
constexpr int I2C_SCL = 10;

// The ESP pins needed for the LoRa module
constexpr int LORA_DIO0 = 0;
constexpr int LORA_RESET = 1;
constexpr int LORA_SCK = 4;
constexpr int LORA_MISO = 5;
constexpr int LORA_MOSI = 6;
constexpr int LORA_NSS = 7;

constexpr float SEA_LEVEL_PRESSURE = 1013.25F; // The pressure at sea level (in hPa)
constexpr long LORA_BAND = 868E6; // The radio band that our module will use; Change this and you go to jail :)

float takeOffAltitude; // A constant compiled when the program starts, showing where the BME280 is at the start compared to sea level

// State variables, in case the BME or LoRa aren't connected properly
bool bmeConnected = true;
bool loraConnected = true;

void setup() {
  // Initialize the ESP at Serial por 115200, and printing that a new session started
  Serial.begin(115200);
  delay(500);
  Serial.println("\n\n\nNew session started");

  Wire.begin(I2C_SDA, I2C_SCL); // Setting up the Wire protocol for the BME
  bmeConnected = bme.begin(0x76); // Initializing the BME
  if (!bmeConnected) { // If the BME isn't conencted, throw an error
    Serial.println("No BME280 sensor over here, pls fix your wiring(");
  } else { // Else, initialize the BME
    Serial.println("BME280 initialized");
    
    takeOffAltitude = bme.readAltitude(SEA_LEVEL_PRESSURE); // Initializa the take off altitude
    Serial.print("Takeoff Altitude: "); Serial.print(takeOffAltitude); Serial.println(" m\n"); // Print it
  }

  // Initialize the SPI protocol for the LoRa module
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);
  LoRa.setPins(LORA_NSS, LORA_RESET, LORA_DIO0); // Set the LoRa's pins
  if (!LoRa.begin(LORA_BAND)) { // If the LoRa isn't initalized properly, give an error
    Serial.println("No LoRa module over here, pls fix your wiring(");
    loraConnected = false;
  } else {
    LoRa.setSyncWord(0xF3); // Set the LoRa's sync word
    Serial.println("LoRa initialized");
  }
}

int packetNr = 1; // Start counting packets
void loop() {
  if (bmeConnected) {
    // Use the bme's built-in functions to get temperature, pressure, humidity and altitude
    float temperature = bme.readTemperature();
    float pressure = bme.readPressure() / 100.0F;
    float humidity = bme.readHumidity();
    float seaAltitude = bme.readAltitude(SEA_LEVEL_PRESSURE);
    float groundAltitude = seaAltitude - takeOffAltitude; // Calculate altitude relative to ground

    // Print all the values
    Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" C");
    Serial.print("Pressure: "); Serial.print(pressure); Serial.println(" hPa");
    Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
    Serial.print("Altitude (sea): "); Serial.print(seaAltitude); Serial.println(" m");
    Serial.print("Altitude (relative to ground): "); Serial.print(groundAltitude); Serial.println(" m");

    if (loraConnected) {
      LoRa.beginPacket(); // Start a packet
        // Have the LoRa send the information
        LoRa.print(temperature); LoRa.print(" "); 
        LoRa.print(pressure); LoRa.print(" "); 
        LoRa.print(humidity); LoRa.print(" "); 
        LoRa.print(seaAltitude); LoRa.print(" "); 
        LoRa.println(groundAltitude);
      LoRa.endPacket(); // End the packet

      Serial.print("Packet nr."); Serial.print(packetNr); Serial.println(" sent"); Serial.print("\n"); // Print the packet number
      packetNr++; // Increment the packet number
    }
  }

  delay(500); // Add a 500ms delay, since we want to send stuff every half a second
}