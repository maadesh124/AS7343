#ifndef AS7343_H
#define AS7343_H

#include <Wire.h>
#define POWER 0x80
#define ATIME 0x81
#define ASTEP0 0xD4
#define ASTEP1 0xD5
#define AGAIN 0xC6
#define SMUX 0xD6
#define LED 0xCD

class AS7343
{
private:
    /* data */
    uint8_t address;
public:
    AS7343(uint8_t address=0x39);
    ~AS7343();
    void setLED(bool state);
bool begin(uint16_t astep=0x0257,uint8_t atime=0x3B,uint8_t again=0x07);
   uint8_t writeRegister(uint8_t reg,uint8_t val);
   uint8_t readRegister(uint8_t reg);
   uint16_t read16(uint8_t reg);
   bool readData(uint16_t* data,uint8_t startReg,uint8_t n);
   bool readAllChannels(uint16_t* data);
    void printData(uint16_t* data,int n);

uint16_t readChannel(uint8_t n);


};









#endif