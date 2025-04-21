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
  // Serial.println("Dallas Temperature IC Control Library Demo"); 
  sensors.begin();
  
  // Serial.print("Locating devices...");
  // Serial.print("Found ");
  // Serial.print(sensors.getDeviceCount(), DEC);
  // Serial.println(" devices.");

  // Serial.print("Parasite power is: ");
  // if(sensors.isParasitePowerMode()) {
  //   Serial.println("ON"); } 
  // else {
  //   Serial.println("OFF");
  // }

// one tab 
  if(!sensors.getAddress(one, 0)) {
    Serial.println("Unable to find address for Device 0");
  }
  
  // if(!sensors.getAddress(two, 2)) { 
  //   Serial.println("Unable to find address for Device 2"); 
  // } 
  
  // if(!sensors.getAddress(three, 1)) { 
  //   Serial.println("Unable to find address for Device 1");
  // }

  // Serial.print("Device 0 Address: "); 
  // printAddress(one);
  // Serial.println(); 
  
  //   Serial.print("Device 1 Address: ");
  // printAddress(two); Serial.println();
  // Serial.print("Device 2 Address: "); printAddress(three);
  // Serial.println(); 

  sensors.setResolution(one, TEMPERATURE_PRECISION); 

  // sensors.setResolution(two,
  // TEMPERATURE_PRECISION); sensors.setResolution(three,
  // TEMPERATURE_PRECISION); 

  // Serial.print("Device 0 Resolution: ");
  // Serial.print(sensors.getResolution(one), DEC);
  // Serial.println();
  
//  Serial.print("Device 1 Resolution: ");
//  Serial.print(sensors.getResolution(two), DEC);
//  Serial.println();
//  Serial.print("Device 2 Resolution: ");
//  Serial.print(sensors.getResolution(three), DEC);
//  Serial.println();
}


void printAddress(DeviceAddress deviceAddress) { 
  for (uint8_t i = 0; i < 8; i++) { 
    if(deviceAddress[i] < 16) {
      Serial.print("0"); 
      Serial.print(deviceAddress[i], HEX);
    } 
  } 
}

void printTemperature(DeviceAddress deviceAddress) {
  float tempC = sensors.getTempC(deviceAddress);
  // Serial.print("Temp: ");
  Serial.print(tempC);
  // Serial.print(" C; ");
  // Serial.print(DallasTemperature::toFahrenheit(tempC));
  // Serial.print(" F"); 
}

void printResolution(DeviceAddress deviceAddress) {
  Serial.print("Resolution: ");
  Serial.println(sensors.getResolution(deviceAddress)); 
} 

void printData(DeviceAddress deviceAddress) {
  printTemperature(deviceAddress); 
  // Serial.print(" ; Device: ");
  // printAddress(deviceAddress); 
  Serial.println();
}

void loop() { 
  // Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  // Serial.println("DONE"); 
  printData(one);
  // printData(two); printData(three);
  delay(1000); // every 1 second
}
