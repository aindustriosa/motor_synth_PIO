// manages motors: setup and set speed

#ifndef _MOTOR_SYNTH_MOTORCONTROLLER_H_
#define _MOTOR_SYNTH_MOTORCONTROLLER_H_

#include <Arduino.h>
#ifdef NATIVE
#include "FakeServo.h"
#else
#include <Servo.h>
#endif // NATIVE

class MotorController
{
public:
    void setup(int control_pin);
    void setSpeed(int speed); // speed goes from 0 to 1000
    int getMaxSpeed(); // returns the maximun speed available for this motor

private:
    Servo servo;
    int control_pin = 9;
    int servoMinValue = 1000;
    int servoMaxValue = 2000;
};

#endif