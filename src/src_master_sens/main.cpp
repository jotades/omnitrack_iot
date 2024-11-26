#include <SPI.h>
#include <ArduinoJson.h> // Include ArduinoJson library
#include "DW1000Ranging.h"
#include "DW1000.h"

uint16_t shortAddress = 132; //84:00
const char *macAddress = "5B:D5:A9:9A:E2:9C";

// Calibrated Antenna Delay setting for this anchor
uint16_t Adelay = 16580;

// Calibration distance
float dist_m = (285 - 1.75) * 0.0254; // meters

#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4

// Connection pins
const uint8_t PIN_RST = 27; // Reset pin
const uint8_t PIN_IRQ = 34; // IRQ pin
const uint8_t PIN_SS = 4;   // SPI select pin

// Mode parameters for DW1000
static constexpr byte MY_MODE[] = {DW1000.TRX_RATE_6800KBPS, DW1000.TX_PULSE_FREQ_16MHZ, DW1000.TX_PREAMBLE_LEN_64};

void sendJsonToSerial(const char *event, const char *key, float value, const char *address = nullptr) {
  StaticJsonDocument<256> doc; // JSON document with a fixed buffer size

  doc["event"] = event;         // Add event type
  if (address != nullptr) {
    doc["address"] = address;   // Add device address if available
  }
  doc[key] = value;             // Add value with its key

  serializeJson(doc, Serial);   // Serialize and send JSON to Serial
  Serial.println();             // Add a newline for readability
}

void newRange(DW1000Device *device)
{
  Serial.print("from: ");
  Serial.print(device->getShortAddress(), HEX);
  Serial.print(", ");

#define NUMBER_OF_DISTANCES 1
  float dist = 0.0;
  for (int i = 0; i < NUMBER_OF_DISTANCES; i++) {
        dist += device->getRange();
}
  dist = dist/NUMBER_OF_DISTANCES;
  // Serial.println(dist);
  // Send JSON document
 sendJsonToSerial("new_range", "distance", dist, String(device->getShortAddress(), HEX).c_str());
}
void newDevice(DW1000Device *device)
{
  // Send JSON document
  sendJsonToSerial("new_device", "added", 1, String(device->getShortAddress(), HEX).c_str());
}

void inactiveDevice(DW1000Device *device)
{
  // Send JSON document
  sendJsonToSerial("inactive_device", "removed", 1, String(device->getShortAddress(), HEX).c_str());
}

void setup()
{
  Serial.begin(115200);
  delay(1000); // Wait for serial monitor to connect
  Serial.println("Anchor config and start");

  // Send initial setup information as JSON
  sendJsonToSerial("setup", "antenna_delay", Adelay);
  sendJsonToSerial("setup", "calibration_distance", dist_m);

  // Init the configuration
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  DW1000Ranging.init(BoardType::ANCHOR, shortAddress, macAddress, false, MY_MODE, PIN_RST, PIN_SS, PIN_IRQ);

  // Set antenna delay for anchors
  DW1000.setAntennaDelay(Adelay);

  // Attach callback functions
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);
}

void loop()
{
  DW1000Ranging.loop();
}
