#pragma once
//stub for arduino spi class

#include "Arduino.h"
class SPISettings{
public:
  int arf;
};
class SPI {
public:
void beginTransaction(const SPISettings&settings);
void endTransaction();
void begin();
uint8_t transfer( uint8_t outgoing);

};

extern SPI SPI1;
