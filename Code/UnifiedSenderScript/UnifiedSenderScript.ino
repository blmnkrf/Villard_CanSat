#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

#include <SPI.h>
#include <LoRa.h>

Adafruit_BME280 bme;

constexpr int I2C_SDA = 3;
constexpr int I2C_SCL = 10;

constexpr int LORA_DIO0 = 0;
constexpr int LORA_RESET = 1;
constexpr int LORA_SCK = 4;
constexpr int LORA_MISO = 5;
constexpr int LORA_MOSI = 6;
constexpr int LORA_NSS = 7;

constexpr float SEA_LEVEL_PRESSURE = 1013.25F;
constexpr long LORA_BAND = 868E6; // change this and u go to jail :)

float takeOffAltitude;

bool bmeConnected = true;
bool loraConnected = true;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n\n\nNew session started");

  Wire.begin(I2C_SDA, I2C_SCL);
  bmeConnected = bme.begin(0x76);
  if (!bmeConnected) {
    Serial.println("No BME280 sensor over here, pls fix your wiring(");
  } else {
    Serial.println("BME280 initialized");
    
    takeOffAltitude = bme.readAltitude(SEA_LEVEL_PRESSURE);
    Serial.print("Takeoff Altitude: "); Serial.print(takeOffAltitude); Serial.println(" m\n");
  }

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);
  LoRa.setPins(LORA_NSS, LORA_RESET, LORA_DIO0);
  if (!LoRa.begin(LORA_BAND)) {
    Serial.println("No LoRa module over here, pls fix your wiring(");
    loraConnected = false;
  } else {
    LoRa.setSyncWord(0xF3);
    Serial.println("LoRa initialized");
  }
}

int packetNr = 1;
void loop() {
  if (bmeConnected) {
    float temperature = bme.readTemperature();
    float pressure = bme.readPressure() / 100.0F;
    float humidity = bme.readHumidity();
    float seaAltitude = bme.readAltitude(SEA_LEVEL_PRESSURE);
    float groundAltitude = seaAltitude - takeOffAltitude;

    Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" C");
    Serial.print("Pressure: "); Serial.print(pressure); Serial.println(" hPa");
    Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
    Serial.print("Altitude (sea): "); Serial.print(seaAltitude); Serial.println(" m");
    Serial.print("Altitude (relative to ground): "); Serial.print(groundAltitude); Serial.println(" m");

    if (loraConnected) {
      LoRa.beginPacket();
        LoRa.println(temperature);
        LoRa.println(pressure);
        LoRa.println(humidity);
        LoRa.println(seaAltitude);
        LoRa.println(groundAltitude);
      LoRa.endPacket();

      Serial.print("Packet nr."); Serial.print(packetNr); Serial.println(" sent"); Serial.print("\n");
      packetNr++;
    }
  }

  delay(500);
}