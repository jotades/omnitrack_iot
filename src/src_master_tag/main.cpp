#include <ArduinoJson.h>
#include <SPI.h>
#include <DW1000Ranging.h>
#include "link.h"
#include "bluetooth_module.h"
// #include "tdma.h"

// DEFINES
#define IS_ANCHOR
//#define IS_TAG

#ifdef IS_TAG
#define DEVICE_ADDRESS "03:00:00:00:00:00:30:03"
struct MyLink *uwb_data;
unsigned long lastUpdateTime = 0;
unsigned int updateInterval = 500;
#endif

#ifdef IS_ANCHOR
#define DEVICE_ADDRESS "02:00:00:00:00:00:20:02"
uint16_t Adelay = 16580;//16681;//16534//16535//16495
struct MyLink *uwb_data;
unsigned long lastUpdateTime = 0;
unsigned int updateInterval = 500;
#endif

char shortAddress[6];

#ifdef IS_TAG
void send_json(struct MyLink *p) {
  StaticJsonDocument<512> doc;
  doc["Tag"] = DEVICE_ADDRESS;

  JsonArray links = doc.createNestedArray("links");
  struct MyLink *temp = p->next;
  while (temp != NULL) {
    JsonObject obj = links.createNestedObject();
    obj["anchor"] = temp->anchor_addr;
    
    // Divide the range by 2 before formatting it as a string
    float dividedRange = temp->range[0] ;// -/ 2.0f
    
    char range[8];
    sprintf(range, "%.2f", dividedRange);
    obj["range"] = range;
    
    temp = temp->next;
  }

  serializeJson(doc, Serial);
  Serial.println("");
  updateBluetoothData(doc.as<JsonObject>());
}
#endif

#ifdef IS_ANCHOR
void send_anchor_json(struct MyLink *p) {
  StaticJsonDocument<512> doc;
  doc["Anchor"] = DEVICE_ADDRESS;

  JsonArray tags = doc.createNestedArray("tags");
  struct MyLink *temp = p->next;
  while (temp != NULL) {
    JsonObject obj = tags.createNestedObject();
    obj["tag"] = temp->anchor_addr;
    char range[8];
    sprintf(range, "%.2f", temp->range[0]);
    obj["distance"] = range;
    temp = temp->next;
  }
  serializeJson(doc, Serial);
  Serial.println("");
  updateBluetoothData(doc.as<JsonObject>());
}
#endif

void newRange() {
  //Serial.print("From: ");
  // Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  // Serial.print(", Range: ");
  // Serial.print(DW1000Ranging.getDistantDevice()->getRange());
  // Serial.println("m");


  update_link(uwb_data, DW1000Ranging.getDistantDevice()->getShortAddress(), DW1000Ranging.getDistantDevice()->getRange(), DW1000Ranging.getDistantDevice()->getRXPower());
  //update_link(uwb_data, DW1000Ranging.getDistantDevice()->getShortAddress(), DW1000Ranging.getDistantDevice()->getRange(), 0);
}

void newDevice(DW1000Device *device) {
  Serial.print("New device detected: ");
  Serial.println(device->getShortAddress(), HEX);
  add_link(uwb_data, device->getShortAddress());
}

void inactiveDevice(DW1000Device *device) {
  Serial.print("Device removed: ");
  Serial.println(device->getShortAddress(), HEX);
  delete_link(uwb_data, device->getShortAddress());
}

void setup() {
  Serial.begin(115200);
  initializeBluetooth();
  SPI.begin(18, 19, 23);
  DW1000Ranging.initCommunication(27, 4, 34);
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);
  
  #ifdef IS_ANCHOR
    DW1000Ranging.startAsAnchor(DEVICE_ADDRESS, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);
    DW1000.setAntennaDelay(Adelay);
    uwb_data = init_link();
    Serial.println("ANCHOR mode");
  #elif defined(IS_TAG)
    DW1000Ranging.startAsTag(DEVICE_ADDRESS, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);
    uwb_data = init_link();
    Serial.println("TAG mode");
  #endif

  byte *currentShortAddress = DW1000Ranging.getCurrentShortAddress();
  sprintf(shortAddress, "%02X%02X", currentShortAddress[1], currentShortAddress[0]);
  Serial.print("Short Address: ");
  Serial.println(shortAddress);
  Serial.println("Setup complete");
}
void loop() {
  DW1000Ranging.loop();

  #ifdef IS_ANCHOR
    if ((millis() - lastUpdateTime) > updateInterval) {
      send_anchor_json(uwb_data);
      lastUpdateTime = millis();
    }
  // #elif defined(IS_TAG)
  //   if (isMySlot() && (millis() - lastUpdateTime) > updateInterval) {
  //     send_json(uwb_data);
  //     lastUpdateTime = millis();
  //     delay(SLOT_DURATION_MS - 1);  // Ensure no overlap in slots
  //   }
  // #endif
  #elif defined(IS_TAG)
    if ((millis() - lastUpdateTime) > updateInterval) {
      send_json(uwb_data);
      lastUpdateTime = millis();
    }
  #endif
}
// // Implement the isAnchor function to check if the device is the anchor
// bool isAnchor(DW1000Device *device) {
//     // Anchor's address
//     return device->getShortAddress() == "01:00:00:00:00:00:10:01";  
// }

//--NOTE--
//How to find the ttyACM port//ls -l /dev/ttyUSB* /dev/ttyACM*

//            V | Test 2 anchor 1 Tag
//            V | Test bluetooth comunication on this code
//           XV | Test adelay value
//Monday-->   X | Test 2 Tag 1 Anchor