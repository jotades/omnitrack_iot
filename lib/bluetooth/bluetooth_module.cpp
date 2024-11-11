#include "bluetooth_module.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>

BLECharacteristic *pCharacteristic;

void initializeBluetooth() {
  BLEDevice::init("jotadesu");
  BLEServer *pServer = BLEDevice::createServer();
  
  // Create a BLE service with a given UUID
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  // Create a BLE characteristic with a given UUID and properties (notify only)
  pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
  
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());
  
  // Start the BLE service
  pService->start();
  
  // Start advertising the BLE service
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  
  Serial.println("Waiting for client connection to notify...");
  delay(1000);  // Allow time for Bluetooth initialization
}

void updateBluetoothData(const String &data) {
  if (pCharacteristic) {
    pCharacteristic->setValue(data.c_str());
    Serial.println("Sending notification...");
    pCharacteristic->notify();
  }
}