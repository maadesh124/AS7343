#include <Arduino.h>
#include "AS7343.h"


AS7343::AS7343(uint8_t address)
{
    this->address=address;
}

AS7343::~AS7343()
{
}



bool AS7343::begin(uint16_t astep,uint8_t atime,uint8_t again)
{
    Wire.begin();
    //check if device is connected

     Wire.beginTransmission(address);
    if(Wire.endTransmission()!=0)
    return false;
    
    if(writeRegister(POWER,0x01)!=0)
    return false;

    delay(5);

    uint8_t msb=(astep>>8) & 0xFF;
    uint8_t lsb=astep & 0xFF;
    if(writeRegister(ASTEP1,msb)!=0)
    return false;

    if(writeRegister(ASTEP0,lsb)!=0)
    return false;

    if(writeRegister(ATIME,atime)!=0)
    return false;

    if(writeRegister(AGAIN,again)!=0)
    return false;
    
    return true;
}

uint8_t AS7343::writeRegister(uint8_t reg,uint8_t val)
{
    Wire.beginTransmission(address);

            Wire.write(reg);
            Wire.write(val);
   return Wire.endTransmission();
}

uint8_t AS7343::readRegister(uint8_t reg)
{
  Wire.beginTransmission(address);
  Wire.write(reg);          
  Wire.endTransmission(false);   

  Wire.requestFrom((uint8_t)address,(uint8_t) 1); 
  if (Wire.available()) {
    return Wire.read();         
  }


  return 0;
}


uint16_t AS7343::read16(uint8_t reg)
{
    Wire.beginTransmission(address);
    Wire.write(reg);              
    Wire.endTransmission(false);       

    Wire.requestFrom((uint8_t)address, (uint8_t)2);

    uint8_t lo = 0, hi = 0;
    if (Wire.available() >= 2) {
        lo = Wire.read();              
        hi = Wire.read();             
    }
    else
      return 0;
    
    

    return (hi << 8) | lo;
}

bool AS7343::readData(uint16_t* data,uint8_t startReg,uint8_t n)
{
  if(data==nullptr)
  return false;
  Wire.beginTransmission(address);
    Wire.write(startReg);              
   if (Wire.endTransmission(false) != 0) return false;       

    Wire.requestFrom((uint8_t)address, (uint8_t)(2*n));

    if(Wire.available() >=2*n)
    {
      for(int i=0;i<n;i++)
      {
        uint8_t lo=Wire.read();
        uint8_t hi=Wire.read();
        data[i]=(hi << 8) | lo;
      }
    }else
    return false;
    
return true;
}


void AS7343::printData(uint16_t* data,int n)
{
  Serial.println("Values in data regs");
  for(int i=0;i<n;i++)
   Serial.println(data[i]);
}

bool AS7343::readAllChannels(uint16_t* data)
{

  if(writeRegister(0xD6,0x60)!=0)
  return false;

  if(writeRegister(POWER,0x03)!=0)
  return false;

  delay(5);

  while((readRegister(0x90) & 0x40) == 0 );

  if(readData(data,0x95,9)==false)
  return false;
  if(readData(data+9,0xA7,9)==false)
  return false;
  return true;
}


uint16_t AS7343::readChannel(uint8_t n)
{
  if(n<0 || n>17)
  return 0;

 
  if(writeRegister(0xD6,0x60)!=0)
  return 0;

  if(writeRegister(POWER,0x03)!=0)
  return 0;

  delay(5);


  while((readRegister(0x90) & 0x40) == 0 );

  
  uint8_t regAddress=0x95 + (2*n);
  return read16(regAddress);
}