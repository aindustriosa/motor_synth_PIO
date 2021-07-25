// manages the serial port: setup and read/write

#ifndef _MOTOR_SYNTH_SERIAL_H_
#define _MOTOR_SYNTH_SERIAL_H_

#include <stdio.h> // for size_t

namespace motor_synth
{
    struct SerialIO
    {
        virtual void print(const char *text) = 0;
        virtual size_t print(int n, int base) = 0;
        virtual size_t println() = 0;
        virtual size_t println(unsigned int n, int base) = 0;
        virtual size_t println(const char *text) = 0;
        virtual void println(const char *text, int number) = 0;
    };
} // namespace motor_synth

#endif //_MOTOR_SYNTH_SERIAL_H_