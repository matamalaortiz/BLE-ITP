#include <CurieBLE.h>

// create peripheral instance
BLEPeripheral blePeripheral;

// create service
BLEService buttonService = BLEService("AA00");

// create characteristic
BLECharCharacteristic buttonCharacteristic = BLECharCharacteristic("AA01", BLENotify);
BLEDescriptor buttonDescriptor = BLEDescriptor("2901", "Button State");

#define BUTTON_PIN 7

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);

  // set advertised local name and service UUID
  blePeripheral.setLocalName("ButtonAlejandro");
  blePeripheral.setDeviceName("ButtonAlejandro");
  blePeripheral.setAdvertisedServiceUuid(buttonService.uuid());

  // add service and characteristics
  blePeripheral.addAttribute(buttonService);
  blePeripheral.addAttribute(buttonCharacteristic);
  blePeripheral.addAttribute(buttonDescriptor);

  // begin initialization
  blePeripheral.begin();
  Serial.println(F("Bluetooth Button"));
}

void loop() {
  // Tell the bluetooth radio to do whatever it should be working on	
  blePeripheral.poll();

  char buttonValue = digitalRead(BUTTON_PIN);

  // has the value changed since the last read?
  if (buttonCharacteristic.value() != buttonValue) {
    Serial.print("Button ");
    Serial.println(buttonValue, HEX);
    buttonCharacteristic.setValue(buttonValue);
  }
}
