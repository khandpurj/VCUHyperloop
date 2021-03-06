#include <OneWire.h>

#define ONE_WIRE_BUS 2
#define TEMP_PRECISION 9
#define NUMBER_OF_DS18B20 30

#define DELAY 500

#define SERIAL 1
#define COMPARE 1
#define LOG 1

OneWire oneWire(ONE_WIRE_BUS); // DS18B20 sensors on pin defined by ONE_WIRE_BUS

byte DS18B20DataArray[12][NUMBER_OF_DS18B20];
float tempDataArray[NUMBER_OF_DS18B20];

byte sensorAddress[NUMBER_OF_DS18B20][8] = {
/*Sensor 0*/   {0x28,0x4,0x77,0xD3,0x8,0x0,0x0,0x52},
/*Sensor 1*/   {0x28,0x29,0xCD,0xD4,0x8,0x0,0x0,0xDA},
/*Sensor 2*/   {0x28,0x53,0x87,0xD3,0x8,0x0,0x0,0x5F},
/*Sensor 3*/   {0x28,0xB3,0xC0,0xD3,0x8,0x0,0x0,0xCE},
/*Sensor 4*/   {0x28,0x88,0xD5,0xD5,0x8,0x0,0x0,0x7C},
/*Sensor 5*/   {0x28,0x38,0x9,0xD4,0x8,0x0,0x0,0x8B},
/*Sensor 6*/   {0x28,0xAC,0xAF,0xD5,0x8,0x0,0x0,0xC6},
/*Sensor 7*/   {0x28,0xF9,0x2A,0xD2,0x8,0x0,0x0,0x9C},
/*Sensor 8*/   {0x28,0xDD,0x80,0xD2,0x8,0x0,0x0,0x78},
/*Sensor 9*/   {0x28,0xFF,0xBE,0xC8,0xC1,0x16,0x4,0x5F},
/*Sensor 10*/  {0x28,0xEB,0xCE,0x8F,0x9,0x0,0x0,0x82},
/*Sensor 11*/  {0x28,0xB1,0x1E,0x8D,0x9,0x0,0x0,0x3A},
/*Sensor 12*/  {0x28,0x5E,0x0,0x8F,0x9,0x0,0x0,0xEE},
/*Sensor 13*/  {0x28,0x14,0xD4,0x8C,0x9,0x0,0x0,0x9D},
/*Sensor 14*/  {0x28,0xE5,0xC7,0x8F,0x9,0x0,0x0,0x62},
/*Sensor 15*/  {0x28,0xF5,0x90,0x8C,0x9,0x0,0x0,0x75},
/*Sensor 16*/  {0x28,0xF5,0x90,0x8C,0x9,0x0,0x0,0x75},
/*Sensor 17*/  {0x28,0x6F,0xB7,0x8F,0x9,0x0,0x0,0x2A},
/*Sensor 18*/  {0x28,0xF8,0xC5,0x8F,0x9,0x0,0x0,0xF0},
/*Sensor 19*/  {0x28,0x93,0x63,0x8D,0x9,0x0,0x0,0x63},
/*Sensor 20*/  {0x28,0x90,0xFD,0x8C,0x9,0x0,0x0,0xA0},
/*Sensor 21*/  {0x28,0x5C,0x8F,0x8F,0x9,0x0,0x0,0x24},
/*Sensor 22*/  {0x28,0xAA,0x68,0x8E,0x9,0x0,0x0,0xE0},
/*Sensor 23*/  {0x28,0x9,0x82,0x8F,0x9,0x0,0x0,0x0D},
/*Sensor 24*/  {0x28,0xA0,0x3B,0x8F,0x9,0x0,0x0,0x7B},
/*Sensor 25*/  {0x28,0xDC,0xA8,0x8C,0x9,0x0,0x0,0xEF},
/*Sensor 26*/  {0x28,0xD1,0x8D,0x8C,0x9,0x0,0x0,0x8F},
/*Sensor 27*/  {0x28,0xFA,0xAC,0x8F,0x9,0x0,0x0,0x7C},
/*Sensor 28*/  {0x28,0x5A,0x30,0x8E,0x9,0x0,0x0,0x39},
/*Sensor 29*/  {0x28,0x2F,0x26,0x8D,0x9,0x0,0x0,0x22}
  };

void getDS18B20Data(byte *sensorAddressPointer[NUMBER_OF_DS18B20], byte *DS18B20DataArrayPointer[NUMBER_OF_DS18B20]){
  for(uint16_t deviceIndex = 0; deviceIndex < NUMBER_OF_DS18B20; ++deviceIndex){
    oneWire.reset();
    oneWire.select(sensorAddressPointer[deviceIndex]);
    oneWire.write(0x44); // Initiate temperature conversion
  }
  
  delay(DELAY);     // maybe 750ms is enough, maybe not

  for(uint16_t deviceIndex = 0; deviceIndex < NUMBER_OF_DS18B20; ++deviceIndex){
    oneWire.reset();
    oneWire.select(sensorAddressPointer[deviceIndex]);    
    oneWire.write(0xBE);         // Read Scratchpad
    for ( uint8_t i = 0; i < 9; i++) {           // we need 9 bytes
       DS18B20DataArrayPointer[i][deviceIndex]= oneWire.read();
    }
  }
}


void setup() {
  Serial.begin(9600);
}


void loop() {
  
  
  
  
  for(uint16_t deviceIndex = 0; deviceIndex < NUMBER_OF_DS18B20; ++deviceIndex){
    int16_t raw = (DS18B20DataArray[1][deviceIndex] << 8) | DS18B20DataArray[0][deviceIndex];
  
    byte cfg = (DS18B20DataArray[4][deviceIndex] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time

    tempDataArray[deviceIndex] = (float)raw / 16.0;
  }
}
