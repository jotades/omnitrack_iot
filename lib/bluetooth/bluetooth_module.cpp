#include "bluetooth_module.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <ArduinoJson.h>

BLECharacteristic *pCharacteristic;

void initializeBluetooth() {
    BLEDevice::init("jo");
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

void updateBluetoothData(const JsonObject &jsonData) {
    // Serialize JSON object to string
    String jsonString;
    serializeJson(jsonData, jsonString);
    
    // Set the value and send notification
    pCharacteristic->setValue(jsonString.c_str());
    Serial.println("Sending notification:");
    Serial.println(jsonString);
    pCharacteristic->notify();
}
