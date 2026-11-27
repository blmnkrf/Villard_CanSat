// Import the libraries needed for the LoRa module, including the SPI protocol
#include <SPI.h>
#include <LoRa.h>
#include <math.h> // Include the math library

// I'm using constexpr instead of #define since it is actually compile time and saves RAM

// The ESP pins needed for the LoRa module
constexpr int LORA_DIO0 = 0;
constexpr int LORA_RESET = 1;
constexpr int LORA_SCK = 4;
constexpr int LORA_MISO = 5;
constexpr int LORA_MOSI = 6;
constexpr int LORA_NSS = 7;

constexpr int BUZZER_PIN = 3; // The pin that the buzzer is connected to
float buzzerDelay = 1000; // Initial delay in ms

constexpr int LORA_BAND = 868E6; // The radio band that our module will use; Change this and you go to jail :)

constexpr float ONE_METER_RSSI = -44.0f; // A constant that we measured. It is the RSSI in dBm
constexpr float ENVIRONMENTAL_CONSTANT = 1.84f;

bool loraConnected = true; // State variable, in case the LoRa isnt't connected properly

void setup() {
  // Initialize the ESP at Serial por 115200, and printing that a new session started
  Serial.begin(115200);
  delay(500);
  Serial.println("\n\n\nNew session started");

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS); // Setting up the SPI protocol for the LoRa module
  LoRa.setPins(LORA_NSS, LORA_RESET, LORA_DIO0); // Setting the LoRa's pins

  // Buzzer setup
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  if (!LoRa.begin(LORA_BAND)) { // If the LoRa isn't initalized properly, give an error
    Serial.println("LoRa's wiring ain't working!");
    loraConnected = false;
  }
  else {
    LoRa.setSyncWord(0xF3); // Else, set LoRa's sync word
    Serial.println("LoRa Initialization successful! Waiting for packets...");
  }
}

int packetNr = 1; // Start counting packets
void loop() {
  if (loraConnected) {
    int packetSize = LoRa.parsePacket(); // Parse the packet and get its size

    if (packetSize) { // If the packet size is greater than 0 (aka if we received a packet)
      Serial.print("Received packet "); Serial.println(packetNr); // Print the packet number
      packetNr++; // Increment the packet number

      while (LoRa.available()) { // Print all the data in the packet
        String receivedData = LoRa.readString();
        Serial.print(receivedData);
      }

      float signalStrength = LoRa.packetRssi(); // Get the RSSI (signal strength) (might need to use RSSM_dBm = Rssi - 157 for dBm conversion)
      float distance = pow(10, ((ONE_METER_RSSI - (signalStrength)) / (10 * ENVIRONMENTAL_CONSTANT))); // Logarithmic distance calculation using signal strength
      
      if (distance > 1000) { // If you're too far away, turn off the buzzer
        digitalWrite(BUZZER_PIN, LOW);
      }
      else {
        digitalWrite(BUZZER_PIN, HIGH);
        
        buzzerDelay = distance * 50; // The closer you are, the faster it beeps
        delay(buzzerDelay);
        digitalWrite(BUZZER_PIN, LOW);
        delay(buzzerDelay);
      }
      // Signal strength classification:
      // -30 - -60 -> excelent
      // -70 - -90 -> good
      // -100 - -115 -> weak
      // -120 or worse -> abysmal

      Serial.print("Signal strength: "); Serial.println(signalStrength); // Print the signal strength
      Serial.print("Distance: "); Serial.println(distance); // Print the distance
      
      // Evaluate how good the signal strength is and print it
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