#ifndef FakeEEPROM_h
#define FakeEEPROM_h

#include <inttypes.h>

class EEPROMClass
{
public:
  uint8_t read(int idx) { return 0; }
  void write(int idx, uint8_t val) {}
  void update(int idx, uint8_t val) {}
};


EEPROMClass EEPROM;

#endif