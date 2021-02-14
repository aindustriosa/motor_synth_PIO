// Deals with the in board LED

#ifndef _MOTOR_SYNTH_BLINK_H_
#define _MOTOR_SYNTH_BLINK_H_

#include <Arduino.h>

class Blink
{

public:
    void setup();
    void blink();
    void toggle();

private:
    const int ledPin = 13;
    int ledStatus = LOW;
};

#endif