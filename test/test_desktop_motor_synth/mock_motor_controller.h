#include "motor_controller.h"

// manages a motor: setup and set speed

#ifndef _MOTOR_SYNTH_MOCK_MOTORCONTROLLER_H_
#define _MOTOR_SYNTH_MOCK_MOTORCONTROLLER_H_


namespace motor_synth
{
    class MockMotorController : public MotorController
    {
    public:
        void setup(int control_pin) {}
        void setSpeed(int speed) {
            setSpeed_timesCalled++;
            setSpeed_lastSpeed = speed;
        }
        int getSpeed() {
            getSpeed_timesCalled++;
            return setSpeed_lastSpeed;
        }
        int getMaxSpeed() {return this->max_speed;}  // returns the maximun speed available for this motor

        int max_speed = 1000;
        
        int setSpeed_timesCalled = 0;
        int setSpeed_lastSpeed = 0;
        int getSpeed_timesCalled = 0;
    private:
        int control_pin = 9;
        int servoMinValue = 1000;
        int servoMaxValue = 2000;
    };
} // namespace motor_synth

#endif