
#include "servo_motor_controller.h"

namespace motor_synth
{

void ServoMotorController::setup(int control_pin){
  servo.attach(control_pin, servoMinValue, servoMaxValue);
  servo.write(0);
  delay(5000);
}

void ServoMotorController::setSpeed(int speed){
    int value = speed + servoMinValue;
    if (value < servoMinValue) {
        value = servoMinValue;
    }
    if (value > servoMaxValue) {
        value = servoMaxValue;
    }
    servo.writeMicroseconds(value);
    this->currentSpeed = speed;
}

int ServoMotorController::getMaxSpeed(){
    return servoMaxValue - servoMinValue;
}

} // namespace motor_synth