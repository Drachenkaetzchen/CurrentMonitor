#include <OneWire.h>

OneWire ds(5);  // on pin 10

const int currentIn1 = A0;
const int currentIn2 = A2;
const int voltageIn1 = A1;
const int voltageIn2 = A3;

const int numReadings = 10;

float readings[4][numReadings];
int readingCounter[4];

const float resistorValue = 0.1;
const float voltageFactor = 5.6;
const float currentFactor = 1200;

float ADCFactor = 0.0;
float sensorValue = 0.0;        // value read from the pot

void setup() {
  // initialize serial communications at 9600 bps:
  ADCFactor = 5.0 / 1024.0;
  Serial.begin(9600);
}

void loop() {
  temp();
  Serial.print("0: " );                       
  Serial.print(getVoltage1());
  Serial.print("V");
  Serial.print(" ");
  Serial.print(getCurrent1());
  Serial.print("mA");
  Serial.println();
  
  Serial.print("1: " );                       
  Serial.print(getVoltage2());
  Serial.print("V");
  Serial.print(" ");
  Serial.print(getCurrent2());
  Serial.print("mA");
  Serial.println();

  delay(250);                     
}

float getVoltage1() {
   sensorValue = analogRead(voltageIn1);
   delay(10);

   return (sensorValue * voltageFactor * ADCFactor);
}

float getVoltage2() {
   sensorValue = analogRead(voltageIn2);

   delay(10);  
   return (sensorValue * voltageFactor * ADCFactor);
}

float getCurrent1 () {
  sensorValue = analogRead(currentIn1);
  delay(10);
  return (sensorValue * ADCFactor * currentFactor);
}

float getCurrent2 () {
  sensorValue = analogRead(currentIn2);
  delay(10);
  return (sensorValue * ADCFactor * currentFactor);
}

void temp (void) {
 byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }
  
  for( i = 0; i < 8; i++) {
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      return;
  }
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      type_s = 1;
      break;
    case 0x28:
      type_s = 0;
      break;
    case 0x22:
      type_s = 0;
      break;
    default:
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(100);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("T: ");
  Serial.println(celsius);
}

