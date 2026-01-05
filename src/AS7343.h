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
#define CFG0 0xBF

class AS7343
{
private:
    /* data */
    uint8_t address;
public:
    AS7343(uint8_t address=0x39);
    ~AS7343();
    //sets up AS7343 sensor
    bool begin(uint16_t astep=0x0257,uint8_t atime=0x3B,uint8_t again=0x07);

    //Writes 8bit value to a particular address in AS7343 
    uint8_t writeRegister(uint8_t reg,uint8_t val);

    //Reads a 8 bit register in AS7343
    uint8_t readRegister(uint8_t reg);

    //Reads 2 consecutive addresses starting from reg
    uint16_t read16(uint8_t reg);

    //reads 2*n 8 bit values stored consecutive registers as n 16bit values
    //results are stored in data*
    bool readData(uint16_t* data,uint8_t startReg,uint8_t n);

    //reads all 18 channels and stores the result in data
    bool readAllChannels(uint16_t* data);

    //reads a particular channel n 
    // channels are labelled from 0 to 17.
    uint16_t readChannel(uint8_t n);

    //prints the array of length n
    void printData(uint16_t* data,int n);

};









#endif