// manages the arduino serial port:read/write

#ifndef _MOTOR_SYNTH_ARDUINOSERIAL_H_
#define _MOTOR_SYNTH_ARDUINOSERIAL_H_

#include <Arduino.h>
#include "serial.h"

namespace motor_synth
{
    class ArduinoSerialIO : public SerialIO
    {
    public:
        void print(const char *text)
        {
            Serial.print(text);
        }
        size_t print(int n, int base)
        {
            return Serial.print(n, base);
        }
        size_t println(unsigned int n, int base)
        { 
            return print(n, base) + println();
        }
        size_t println()
        {
            return Serial.println();
        }
        size_t println(const char *text)
        {
            return Serial.println(text);
        }
        void println(const char *text, int number)
        {
            Serial.print(text);
            Serial.println(number);
        }
    };
} // namespace motor_synth

#endif // _MOTOR_SYNTH_ARDUINOSERIAL_H_