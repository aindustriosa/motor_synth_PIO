#ifndef FakeServo_h
#define FakeServo_h

#include <inttypes.h>

class Servo
{
public:
  Servo() {};
  uint8_t attach(int pin) { return 1; };                   // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
  uint8_t attach(int pin, int min, int max) { return 1; }; // as above but also sets min and max values for writes.
  void detach(){};
  void write(int value){};              // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds
  void writeMicroseconds(int value){};  // Write pulse width in microseconds
  int read() { return 0; };             // returns current pulse width as an angle between 0 and 180 degrees
  int readMicroseconds() { return 0; }; // returns current pulse width in microseconds for this servo (was read_us() in first release)
  bool attached() { return true; };
};

#endif //FakeServo_h