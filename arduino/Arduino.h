#ifndef ARDUINO_H
#define ARDUINO_H

//stub for porting to non-arduino environment
#include <stdint.h>

enum {
  HIGH = 1,
  LOW = 0,
  OUTPUT = 1
};

void digitalWrite(int pin, int state);
void pinMode(int pin, int state);

uint16_t millis();

void yield();

class Print {
public:
};
#endif // ARDUINO_H
