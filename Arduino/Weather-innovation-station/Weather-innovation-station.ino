#include <Wire.h>//i2c
#include <BMx280I2C.h>//sensore pressione atmosferica bm280
#include "env_settings.h"

#define BMX280_I2C_ADDRESS 0x76

BMx280I2C bmx280(BMX280_I2C_ADDRESS);

void setup(){
  Serial.begin(115200);
  delay(1000);
  Serial.println(WIFI_SSID);
}

void loop(){
  
}
