#define VIBE_PIN 6 // RedBear Blend & RFduino
// #define VIBE_PIN A3 // RedBear Nano

// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEPeripheral.h>

// define pins (varies per shield/board)
// https://github.com/sandeepmistry/arduino-BLEPeripheral#pinouts
// Blend
#define BLE_REQ     9
#define BLE_RDY     8
#define BLE_RST     5

// create peripheral instance, see pinouts above
BLEPeripheral blePeripheral = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);

// create service
BLEService vibeService = BLEService("FF01");

// create switch and strength characteristic
BLECharCharacteristic switchCharacteristic = BLECharCharacteristic("FF02", BLERead | BLEWrite);
BLEDescriptor switchDescriptor = BLEDescriptor("2901", "Switch");
BLEUnsignedCharCharacteristic strengthCharacteristic = BLEUnsignedCharCharacteristic("FF03", BLERead | BLEWrite);
BLEDescriptor strengthDescriptor = BLEDescriptor("2901", "Strength");

void setup() {
  Serial.begin(9600);

  // set Vibe pin to output mode
  pinMode(VIBE_PIN, OUTPUT);

  // set advertised local name and service UUID
  blePeripheral.setLocalName("Vibe");
  blePeripheral.setDeviceName("Vibe");
  blePeripheral.setAdvertisedServiceUuid(vibeService.uuid());

  // add service and characteristics
  blePeripheral.addAttribute(vibeService);
  blePeripheral.addAttribute(switchCharacteristic);
  blePeripheral.addAttribute(switchDescriptor);
  blePeripheral.addAttribute(strengthCharacteristic);
  blePeripheral.addAttribute(strengthDescriptor);

  // assign event handlers for characteristic
  switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);
  strengthCharacteristic.setEventHandler(BLEWritten, strengthCharacteristicWritten);

  // begin initialization
  blePeripheral.begin();

  Serial.println(F("Bluetooth Vibe"));
}

void loop() {
  // Tell the bluetooth radio to do whatever it should be working on
  blePeripheral.poll();
}

void switchCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  // central wrote new value to characteristic, update Vibe
  Serial.print(F("Characteristic event, written: "));

  if (switchCharacteristic.value()) {
    Serial.println(F("Vibe on"));
    digitalWrite(VIBE_PIN, HIGH);
  } else {
    Serial.println(F("Vibe off"));
    digitalWrite(VIBE_PIN, LOW);
  }
}

void strengthCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  Serial.print(F("strength set to: "));
  Serial.println(strengthCharacteristic.value());

  analogWrite(VIBE_PIN, strengthCharacteristic.value());
}
