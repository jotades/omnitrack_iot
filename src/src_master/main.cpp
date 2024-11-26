#include <SPI.h>
#include "DW1000Ranging.h"

// Anchor address
char anchor_addr[] = "84:00:5B:D5:A9:9A:E2:9C"; //#4
//16495
uint16_t Adelay = 16535;//<--
float prevValue = 0;
float distCent;
float filteredValue;
float dist_m = 0; // meters

#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4

const uint8_t PIN_RST = 27; // reset pin
const uint8_t PIN_IRQ = 34; // irq pin
const uint8_t PIN_SS = 4;   // spi select pin

void newRange()
{
    Serial.print("Anchor:");
    Serial.print("from: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
    Serial.print("\t Range: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getRange());
    Serial.print(" m");
    Serial.print("\t RX power: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getRXPower());
    Serial.println(" dBm");
}

void newDevice(DW1000Device *device)
{
  Serial.print("Device added: ");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device *device)
{
  Serial.print("Delete inactive device: ");
  Serial.println(device->getShortAddress(), HEX);
}

void setup()
{
  Serial.begin(115200);
  delay(1000); // wait for serial monitor to connect
  Serial.println("1-Anchor config and start");
  Serial.print("2-Antenna delay: ");
  Serial.println(Adelay);//<--
  Serial.print("3-Calibration distance: ");
  Serial.println(dist_m);

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ);
  Serial.println("4-DW1000Ranging initialized successfully");

  DW1000.setAntennaDelay(Adelay);//<--

  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);
  Serial.println("5-Callbacks attached");

  DW1000Ranging.startAsAnchor(anchor_addr, DW1000.MODE_LONGDATA_RANGE_LOWPOWER);
  Serial.println("6-Started as anchor");
}
void loop()
{
    DW1000Ranging.loop();
}