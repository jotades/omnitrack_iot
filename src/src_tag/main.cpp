#include <SPI.h>
#include "DW1000Ranging.h"
//#include <Adafruit_I2CDevice.h> for DW3000
//#include <Adafruit_Sensor.h>    for DW3000
#include "bluetooth_module.h"


#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4

char tag_addr[] = "7D:00:22:EA:82:60:3B:9C";

// connection pins
const uint8_t PIN_RST = 27; // reset pin
const uint8_t PIN_IRQ = 34; // irq pin
const uint8_t PIN_SS = 4;   // spi select pin

String data;

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

  updateBluetoothData(data);
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
