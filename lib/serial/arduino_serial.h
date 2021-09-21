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
            delay(DELAY_ms);
        }
        size_t print(int n, int base)
        {
            size_t res = Serial.print(n, base);
            delay(DELAY_ms);
            return res;
        }
        size_t println(unsigned int n, int base)
        { 
            size_t res = print(n, base) + println();
            delay(DELAY_ms);
            return res;
        }
        size_t println()
        {
            size_t res = Serial.println();
            delay(DELAY_ms);
            return res;
        }
        size_t println(const char *text)
        {
            size_t res = Serial.println(text);
            delay(DELAY_ms);
            return res;
        }
        void println(const char *text, int number)
        {
            Serial.print(text);
            Serial.println(number);
            delay(DELAY_ms);
        }

    private:
        uint32_t DELAY_ms = 5;
    };

    
    
    class ArduinoSerialIODoNotPrint : public SerialIO
    {
    public:
        void print(const char *text){}
        size_t print(int n, int base){return 0;}
        size_t println(unsigned int n, int base){return 0;}
        size_t println(){return 0;}
        size_t println(const char *text){return 0;}
        void println(const char *text, int number){}
    };
} // namespace motor_synth

#endif // _MOTOR_SYNTH_ARDUINOSERIAL_H_