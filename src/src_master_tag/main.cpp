#include <BluetoothSerial.h> // For Bluetooth communication
#include <ArduinoJson.h>
#include <SPI.h>
#include <DW1000Ranging.h>
#include "link.h"

// DEFINES
//#define IS_ANCHOR
#define IS_TAG

// CONSTANTS
#define DEVICE_ADDRESS "02:00:00:00:00:00:20:01" //Tag 10:XX  //Anchor 20:XX

// GLOBALS
BluetoothSerial SerialBT; // Bluetooth Serial object
#ifdef IS_TAG
struct MyLink *uwb_data;
unsigned long lastUpdateTime = 0;
unsigned int updateInterval = 200;
#endif

char shortAddress[6];

#ifdef IS_TAG
void send_json(struct MyLink *p) {
 
  StaticJsonDocument<500> doc;

  doc["id"] = shortAddress;

  JsonArray links = doc.createNestedArray("links");
  struct MyLink *temp = p->next; // Skip head node if it's a dummy node
  while (temp != NULL) {
    JsonObject obj1 = links.createNestedObject();
    obj1["a"] = temp->anchor_addr;
    char range[5];
    sprintf(range, "%.2f", temp->range[0]);
    obj1["r"] = range;
    temp = temp->next;
  }

  serializeJson(doc, Serial);
  Serial.println("");

  if (SerialBT.connected()) {
    serializeJson(doc, SerialBT);
    SerialBT.println();
  }
}
#endif

void newRange() {
//   Serial.print("From: ");
//   Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
//   Serial.print(", Range: ");
//   Serial.print(DW1000Ranging.getDistantDevice()->getRange());
//   Serial.println("m");


  #ifdef IS_TAG
    update_link(uwb_data, DW1000Ranging.getDistantDevice()->getShortAddress(), DW1000Ranging.getDistantDevice()->getRange(), DW1000Ranging.getDistantDevice()->getRXPower());
  #endif
 }

void newDevice(DW1000Device *device) {
  Serial.print(F("New device detected! "));
  Serial.println(device->getShortAddress(), HEX);

  #ifdef IS_TAG
    add_link(uwb_data, device->getShortAddress());
  #endif
}

void inactiveDevice(DW1000Device *device) {
  Serial.print(F("Device removed: "));
  Serial.println(device->getShortAddress(), HEX);

  #ifdef IS_TAG
    delete_link(uwb_data, device->getShortAddress());
  #endif
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32-UWBRTLS"); // Set Bluetooth device name
  Serial.println(__FILE__ __DATE__);

  SPI.begin(18, 19, 23);
  DW1000Ranging.initCommunication(27, 4, 34);
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);

  #ifdef IS_ANCHOR
    DW1000Ranging.startAsAnchor(DEVICE_ADDRESS, DW1000.MODE_LONGDATA_RANGE_ACCURACY, false);
    Serial.println("ANCHOR mode");
  #elif defined(IS_TAG)
    DW1000Ranging.startAsTag(DEVICE_ADDRESS, DW1000.MODE_LONGDATA_RANGE_ACCURACY, false);
    uwb_data = init_link();
    Serial.println("TAG mode");
  #endif

  Serial.print(DEVICE_ADDRESS);

  byte* currentShortAddress = DW1000Ranging.getCurrentShortAddress();
  sprintf(shortAddress, "%02X%02X", currentShortAddress[1], currentShortAddress[0]);
  Serial.print(F("Short Address: "));
  Serial.println(shortAddress);

  Serial.println("Setup complete");
}

void loop() {
  DW1000Ranging.loop();

  #ifdef IS_TAG
    if ((millis() - lastUpdateTime) > updateInterval) {
      send_json(uwb_data);
      lastUpdateTime = millis();
    }
  #endif
}

//--NOTE--
//How to find the ttyACM port//ls -l /dev/ttyUSB* /dev/ttyACM*

//Tomorrow--> Test bluetooth comunication on this code
//            Test adelay value


