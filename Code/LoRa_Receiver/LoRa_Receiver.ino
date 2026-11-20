#include <SPI.h>
#include <LoRa.h>
#include <math.h>

constexpr int LORA_DIO0 = 0;
constexpr int LORA_RESET = 1;
constexpr int LORA_SCK = 4;
constexpr int LORA_MISO = 5;
constexpr int LORA_MOSI = 6;
constexpr int LORA_NSS = 7;

constexpr int LORA_BAND = 868E6; // careful on this one :)

constexpr float ONE_METER_RSSI = -44.0f; // dBm
constexpr int ENVIRONMENTAL_CONSTANT = 2.3; // 2 - free space; 2.7-3.5 - with obstacles; 3-5 - indoor

bool loraConnected = true;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.print("\n\n\n\n\n");

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);
  LoRa.setPins(LORA_NSS, LORA_RESET, LORA_DIO0);

  if (!LoRa.begin(LORA_BAND)) {
    Serial.println("LoRa's wiring ain't working!");
    loraConnected = false;
  }
  else {
    LoRa.setSyncWord(0xF3);
    Serial.println("LoRa Initialization successful! Waiting for packets...");
  }
}

int packetNr = 1;
void loop() {
  if (loraConnected) {
    int packetSize = LoRa.parsePacket();

    if (packetSize) {
      Serial.print("Received packet "); Serial.println(packetNr);
      packetNr++;

      while (LoRa.available()) {
        String receivedData = LoRa.readString();
        Serial.print(receivedData);
      }

      float signalStrength = LoRa.packetRssi(); // might need to use RSSM_dBm = Rssi - 157 for dBm conversion
      float distance = pow(10, ((ONE_METER_RSSI - (signalStrength)) / (10 * ENVIRONMENTAL_CONSTANT)));

      // -30 - -60 -> excelent
      // -70 - -90 -> good
      // -100 - -115 -> weak
      // -120 or shittier -> abysmal

      Serial.print("Signal strength: "); Serial.println(signalStrength);
      Serial.print("Distance: "); Serial.println(distance);
      if (signalStrength >= -60) {
        Serial.print("Excelent signal");
      }
      else if (signalStrength >= -90) {
        Serial.print("Good signal");
      }
      else if (signalStrength >= -115) {
        Serial.print("Weak signal");
      }
      else {
        Serial.print("Abysmal signal");
      }
      Serial.println("\n");
    }
  }
}