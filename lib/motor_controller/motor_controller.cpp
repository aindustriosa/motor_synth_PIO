
#include "motor_controller.h"

void MotorController::setup(int control_pin){
  servo.attach(control_pin, servoMinValue, servoMaxValue);
  servo.write(0);
  delay(5000);
}

void MotorController::setSpeed(int speed){
    int value = speed + servoMinValue;
    if (value < servoMinValue) {
        value = servoMinValue;
    }
    if (value > servoMaxValue) {
        value = servoMaxValue;
    }
    servo.writeMicroseconds(value);
}

int MotorController::getMaxSpeed(){
    return servoMaxValue - servoMinValue;
}