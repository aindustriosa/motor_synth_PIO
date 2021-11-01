// manages a servo motor: setup and set speed

#ifndef _MOTOR_SYNTH_SERVOMOTORCONTROLLER_H_
#define _MOTOR_SYNTH_SERVOMOTORCONTROLLER_H_

#include "motor_controller.h"

#include <Arduino.h>
#ifdef NATIVE
#include "../fakes/FakeServo.h"
#else
#include <Servo.h>
#endif // NATIVE

namespace motor_synth
{
    class ServoMotorController : public MotorController
    {
    public:
        void setup(int control_pin);
        void setSpeed(int speed); // speed goes from 0 to 1000
        int getSpeed() {return this->currentSpeed; }
        int getMaxSpeed();        // returns the maximun speed available for this motor

    private:
        Servo servo;
        int control_pin = 9;
        int currentSpeed = 0;
        int servoMinValue = 1000;
        int servoMaxValue = 2000;
    };
} // namespace motor_synth

#endif