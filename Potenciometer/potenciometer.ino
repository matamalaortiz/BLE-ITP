
#include <CurieBLE.h>

BLEPeripheral blePeripheral;
BLEService potentiometerService = BLEService("AAA0");
BLEFloatCharacteristic potentiometerCharacteristic = BLEFloatCharacteristic("AAA1", BLERead | BLENotify);
BLEDescriptor potentiometerDescriptor = BLEDescriptor("2901", "Value");

#define PM_PIN A0

unsigned long previousMillis = 0;  
unsigned short interval = 1000;    

void setup()
{
  Serial.begin(9600);

  // set advertised name and service
  blePeripheral.setLocalName("Potentiometer");
  blePeripheral.setDeviceName("Potentiometer");
  blePeripheral.setAdvertisedServiceUuid(potentiometerService.uuid());

  // add service and characteristic
  blePeripheral.addAttribute(potentiometerService);
  blePeripheral.addAttribute(potentiometerCharacteristic);
  blePeripheral.addAttribute(potentiometerDescriptor);

  blePeripheral.begin();
  
}

void loop()
{
  // Tell the bluetooth radio to do whatever it should be working on
  blePeripheral.poll();

  // limit how often we read the sensor
  if(millis() - previousMillis > interval) {
    pollpotentiometerSensor();
    previousMillis = millis();
  }
}

void pollpotentiometerSensor()
{

  int sensorValue = analogRead(PM_PIN);
     Serial.println(sensorValue);
  int sensorValueMapped = map(sensorValue, 0, 1023, 0, 100);
  
     Serial.print("Sensor Mapped ");
     Serial.println(sensorValueMapped);

  if (potentiometerCharacteristic.value() != sensorValue) {
   potentiometerCharacteristic.setValue(sensorValue);
    Serial.println(sensorValue);
  }
}
