#include <SPI.h>
#include "DW1000Ranging.h"

#include "bluetooth_module.h"

//#include <BLEDevice.h>
//#include <BLEUtils.h>
//#include <BLEServer.h>
//#include <BLE2902.h>

#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4

//#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
//#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

char tag_addr[] = "7D:00:22:EA:82:60:3B:9C";
// connection pins
const uint8_t PIN_RST = 27; // reset pin
const uint8_t PIN_IRQ = 34; // irq pin
const uint8_t PIN_SS = 4;   // spi select pin

String data;

  //Declare pCharacteristic globally
//BLECharacteristic *pCharacteristic;

void newRange()
{
  Serial.print("Tag:");
  Serial.print("from: ");
  Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  Serial.print("\t Range: ");
  Serial.print(DW1000Ranging.getDistantDevice()->getRange());
  Serial.print(" m");
  Serial.print("\t RX power: ");
  Serial.print(DW1000Ranging.getDistantDevice()->getRXPower());
  Serial.println(" dBm");
  // Create a string to hold the range data
  data = "TAG: from: " + String(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX) +
                "\t Range: " + String(DW1000Ranging.getDistantDevice()->getRange()) +
                " m\t RX power: " + String(DW1000Ranging.getDistantDevice()->getRXPower()) + " dBm";

  // Update the BLE characteristic with the range data
  pCharacteristic->setValue(data.c_str());
  // Log the notification sending
  Serial.println("Sending notification...");
  pCharacteristic->notify();
  
}

void newDevice(DW1000Device *device)
{
  Serial.print("ranging init; 1 device added ! -> ");
  Serial.print(" short:");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device *device)
{
  Serial.print("delete inactive device: ");
  Serial.println(device->getShortAddress(), HEX);
}
void setup()
{
  Serial.begin(115200);
      //bluetooth_module.cpp
  Serial.println("READY");
  initializeBluetooth();
  //BLEDevice::init("jotadesu");
  //BLEServer *pServer = BLEDevice::createServer();
      //Create a BLE service with a given UUID
  //BLEService *pService = pServer->createService(SERVICE_UUID);
      //Create a BLE characteristic with a given UUID and properties (read and write)
  //pCharacteristic = pService->createCharacteristic(
                      // CHARACTERISTIC_UUID,
                      // BLECharacteristic::PROPERTY_READ   |
                      // BLECharacteristic::PROPERTY_WRITE  |
                      // BLECharacteristic::PROPERTY_NOTIFY //|
                      // BLECharacteristic::PROPERTY_INDICATE
                      //);
  // Set the initial value of the characteristic to "Hello World"
  // pCharacteristic->setValue("Hello World");
      //Create a BLE Descriptor
  //pCharacteristic->addDescriptor(new BLE2902());
      //Start the BLE service
  //pService->start();
      //Start advertising the BLE service
  //BLEAdvertising *pAdvertising = pServer->getAdvertising();
  //pAdvertising->addServiceUUID(SERVICE_UUID);
  //pServer->getAdvertising()->start();
  //Serial.println("Waiting a client connection to notify...");
  //delay(1000);
  //init the configuration
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin
  
  //define the sketch as anchor. It will be great to dynamically change the type of module
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);
  //Enable the filter to smooth the distancethis.newRange
  DW1000Ranging.useRangeFilter(true);

  //we start the module as a tag
  DW1000Ranging.startAsTag(tag_addr, DW1000.MODE_LONGDATA_RANGE_LOWPOWER);
}

void loop()
{
  DW1000Ranging.loop();
  DW1000Ranging.attachNewRange(newRange);
}
