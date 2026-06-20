/*
  SparkFun ESP32 LoRa Gateway - simple LoRa transmitter

  Target board:
  - SparkFun LoRa Gateway - 1-Channel (ESP32 / RFM95W)

  Wiring is already on the SparkFun board:
  - RFM95 CS  -> GPIO 16
  - RFM95 RST -> GPIO 5
  - RFM95 DIO0/IRQ -> GPIO 26
  - SPI SCK/MISO/MOSI -> GPIO 14 / 12 / 13

  Serial Monitor:
  - 115200 baud
  - Sends one LoRa packet every second at 868 MHz.
*/

#include <SPI.h>
#include <RH_RF95.h>

const uint8_t LORA_SCK_PIN = 14;
const uint8_t LORA_MISO_PIN = 12;
const uint8_t LORA_MOSI_PIN = 13;
const uint8_t LORA_CS_PIN = 16;
const uint8_t LORA_RST_PIN = 5;
const uint8_t LORA_IRQ_PIN = 26;

const float LORA_FREQ_MHZ = 868.0;
const uint8_t LORA_TX_POWER_DBM = 20;
const uint32_t TX_PERIOD_MS = 1000;

RH_RF95 rf95(LORA_CS_PIN, LORA_IRQ_PIN);

uint32_t lastTxMs = 0;
uint32_t packetSeq = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("SparkFun ESP32 LoRa Gateway TX starting...");

  pinMode(LORA_RST_PIN, OUTPUT);
  digitalWrite(LORA_RST_PIN, HIGH);
  delay(10);
  digitalWrite(LORA_RST_PIN, LOW);
  delay(10);
  digitalWrite(LORA_RST_PIN, HIGH);
  delay(10);

  SPI.begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_CS_PIN);

  if (!rf95.init()) {
    Serial.println("ERROR: RFM95 LoRa radio not found. Check board/pins/power.");
    while (true) {
      delay(1000);
    }
  }

  if (!rf95.setFrequency(LORA_FREQ_MHZ)) {
    Serial.println("ERROR: failed to set LoRa frequency.");
    while (true) {
      delay(1000);
    }
  }

  rf95.setTxPower(LORA_TX_POWER_DBM, false);

  Serial.print("LoRa ready at ");
  Serial.print(LORA_FREQ_MHZ, 1);
  Serial.println(" MHz.");
}

void loop() {
  const uint32_t now = millis();
  if (now - lastTxMs < TX_PERIOD_MS) {
    return;
  }
  lastTxMs = now;

  char payload[80];
  snprintf(
    payload,
    sizeof(payload),
    "sparkfun-gateway,seq=%lu,uptime_ms=%lu",
    (unsigned long)packetSeq++,
    (unsigned long)now
  );

  rf95.send((uint8_t *)payload, strlen(payload));
  rf95.waitPacketSent();

  Serial.print("TX: ");
  Serial.println(payload);
}
