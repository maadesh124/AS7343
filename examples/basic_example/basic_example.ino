#include <AS7343.h>

AS7343 sensor(0x39);
uint16_t data[18];
uint8_t regAdd=0xCD,off=0x04,on=0x84;
void setup() {
 Serial.begin(9600);
sensor.begin();

sensor.writeRegister(LED,0x84);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  

}
