// User Interface

#ifndef _MOTOR_SYNTH_CLI_H_
#define _MOTOR_SYNTH_CLI_H_

#include <Arduino.h>

// Internal libs
#include "../blink/blink.h"
#include "../motor_controller/motor_controller.h"

class CLI
{
public:
    void setup();
    void loop();

private:
    Blink blink;           // Board LED management
    MotorController motor; // One motor controller
    const int MOTOR_CONTROL_PIN = 9;

    int getSerialPositiveValue(); // Returns a negative value on failure 
    void print(const char* text);
    void print(const char* text, int number);
    void loop_serial();
    void sweepSpeedUpwards(int delay_between_changes_millis);
    void sweepSpeedDownwards(int delay_between_changes_millis);
    void sweepMotor();
    void waitBlinking();
};

#endif