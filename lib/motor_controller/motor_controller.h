// manages a motor: setup and set speed

#ifndef _MOTOR_SYNTH_MOTORCONTROLLER_H_
#define _MOTOR_SYNTH_MOTORCONTROLLER_H_

namespace motor_synth
{
    struct MotorController
    {
        virtual void setup(int control_pin) = 0;
        virtual void setSpeed(int speed) = 0;
        virtual int getSpeed() = 0;
        virtual int getMaxSpeed() = 0;
    };
} // namespace motor_synth

#endif