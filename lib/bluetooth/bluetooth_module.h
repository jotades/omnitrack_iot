#ifndef BLUETOOTH_MODULE_H
#define BLUETOOTH_MODULE_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <Arduino.h>
#include <ArduinoJson.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

extern BLECharacteristic *pCharacteristic;

void initializeBluetooth();
void updateBluetoothData(const JsonObject &jsonData);

#endif