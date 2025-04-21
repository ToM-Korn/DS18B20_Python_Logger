#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2 // Data wire is plugged into D2 pin
#define TEMPERATURE_PRECISION 9 // +- 0.5 Degree C

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
// DeviceAddress one, two, three;
DeviceAddress one;

void setup() { 
  Serial.begin(9600); 
  sensors.begin();
  if(!sensors.getAddress(one, 0)) {
    Serial.println("Unable to find address for Device 0");
  }

  sensors.setResolution(one, TEMPERATURE_PRECISION); 
}


void printTemperature(DeviceAddress deviceAddress) {
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print(tempC);
}

void printData(DeviceAddress deviceAddress) {
  printTemperature(deviceAddress); 
  Serial.println();
}

void loop() { 
  sensors.requestTemperatures();
  printData(one);
  delay(1000); // every 1 second
}
