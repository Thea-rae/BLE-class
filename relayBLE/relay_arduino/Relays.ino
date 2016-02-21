#define RELAYS_PIN 6 // RedBear Blend & RFduino
// #define RELAYS_PIN A3 // RedBear Nano

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
BLEService vibeService = BLEService("FF01"); //I tried changing these in this and the phonegap code, but then it couldn't find it...

// create switch and strength characteristic
BLECharCharacteristic switch0Characteristic = BLECharCharacteristic("FF02", BLERead | BLEWrite);
BLEDescriptor switch0Descriptor = BLEDescriptor("2901", "Switch");
BLECharCharacteristic switch1Characteristic = BLECharCharacteristic("FF03", BLERead | BLEWrite);
BLEDescriptor switch1Descriptor = BLEDescriptor("2902", "Switch");
BLECharCharacteristic switch2Characteristic = BLECharCharacteristic("FF04", BLERead | BLEWrite);
BLEDescriptor switch2Descriptor = BLEDescriptor("2903", "Switch");
BLECharCharacteristic switch3Characteristic = BLECharCharacteristic("FF05", BLERead | BLEWrite);
BLEDescriptor switch3Descriptor = BLEDescriptor("2904", "Switch");
BLECharCharacteristic switch4Characteristic = BLECharCharacteristic("FF06", BLERead | BLEWrite);
BLEDescriptor switch4Descriptor = BLEDescriptor("2905", "Switch");


void setup() {
  Serial.begin(9600);

  // set Relay pin to output mode
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  // set advertised local name and service UUID
  blePeripheral.setLocalName("Relay");
  blePeripheral.setDeviceName("Relay");
  blePeripheral.setAdvertisedServiceUuid(vibeService.uuid());

  // add service and characteristics
  blePeripheral.addAttribute(vibeService);
  blePeripheral.addAttribute(switch0Characteristic);
  blePeripheral.addAttribute(switch1Characteristic);
  blePeripheral.addAttribute(switch2Characteristic);
  blePeripheral.addAttribute(switch3Characteristic);
  blePeripheral.addAttribute(switch4Characteristic);
  blePeripheral.addAttribute(switch0Descriptor);
  blePeripheral.addAttribute(switch1Descriptor);
  blePeripheral.addAttribute(switch2Descriptor);
  blePeripheral.addAttribute(switch3Descriptor);
  blePeripheral.addAttribute(switch4Descriptor);

  // assign event handlers for characteristic
  switch0Characteristic.setEventHandler(BLEWritten, switch0CharacteristicWritten);
  switch1Characteristic.setEventHandler(BLEWritten, switch1CharacteristicWritten);
  switch2Characteristic.setEventHandler(BLEWritten, switch2CharacteristicWritten);
  switch3Characteristic.setEventHandler(BLEWritten, switch3CharacteristicWritten);
  switch4Characteristic.setEventHandler(BLEWritten, switch4CharacteristicWritten);


  // begin initialization
  blePeripheral.begin();

  Serial.println(F("Bluetooth Relay"));
}

void loop() {
  // Tell the bluetooth radio to do whatever it should be working on
  blePeripheral.poll();
}

void switch0CharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  // central wrote new value to characteristic, update Relay
  Serial.print(F("Characteristic event, written: "));

  if (switch0Characteristic.value()) {
    Serial.println(F("Relay off")); //the board has inverted logic
    digitalWrite(2, HIGH);
  } else {
    Serial.println(F("Relay on"));
    digitalWrite(2, LOW);
  }
}

void switch1CharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  // central wrote new value to characteristic, update Relay
  Serial.print(F("Characteristic event, written: "));

  if (switch1Characteristic.value()) {
    Serial.println(F("Relay off")); //the board has inverted logic
    digitalWrite(3, HIGH);
  } else {
    Serial.println(F("Relay on"));
    digitalWrite(3, LOW);
  }
}

void switch2CharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  // central wrote new value to characteristic, update Relay
  Serial.print(F("Characteristic event, written: "));

  if (switch2Characteristic.value()) {
    Serial.println(F("Relay off")); //the board has inverted logic
    digitalWrite(4, HIGH);
  } else {
    Serial.println(F("Relay on"));
    digitalWrite(4, LOW);
  }
}

void switch3CharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  // central wrote new value to characteristic, update Relay
  Serial.print(F("Characteristic event, written: "));

  if (switch3Characteristic.value()) {
    Serial.println(F("Relay off")); //the board has inverted logic
    digitalWrite(5, HIGH);
  } else {
    Serial.println(F("Relay on"));
    digitalWrite(5, LOW);
  }
}

void switch4CharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  // central wrote new value to characteristic, update Relay
  Serial.print(F("Characteristic event, written: "));

  if (switch4Characteristic.value()) {
    Serial.println(F("Relay off")); //the board has inverted logic
    digitalWrite(RELAYS_PIN, HIGH);
  } else {
    Serial.println(F("Relay on"));
    digitalWrite(RELAYS_PIN, LOW);
  }
}
