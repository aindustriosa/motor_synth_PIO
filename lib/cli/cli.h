// User Interface

#ifndef _MOTOR_SYNTH_CLI_H_
#define _MOTOR_SYNTH_CLI_H_

#include <Arduino.h>

// Internal libs
#include "../blink/blink.h"
#include "../motor_controller/motor_controller.h"
#include "../midi_interface/midi_interface.h"
#include "../data_model/synth_event/synth_event.h"
#include "../motor_synth/motor_synth.h"

class CLI
{
public:
    /**
     * CLI configuration.
     * 
     * Use in the main setup to setup the CLI.
     */
    void setup();
    /**
     * CLI loop.
     * 
     * Presents the main menu and launches the user selection.
     */  
    void loop();

private:
    Blink blink;           // Board LED management
    const int MOTOR_CONTROL_PIN = 9;

    // Main menu commands 
    void menuCommandChangeMotorOnUserSerialInput(int motor_control_pin);
    void menuCommandSweepMotor(int motor_control_pin);
    void menuCommandMidiInterfaceTest();
    void menuCommandMonophonicSynth(int motor_control_pin);

    // Some helpers
    /**
     * Waits for the user to enter a positive number. Returns it.
     * Returns a negative value on failure.
     */
    int getSerialPositiveValue();
    /**
     * Prints on serial port the given text.
     */
    void print(const char* text);
    /**
     * Prints on serial port the given text and the number.
     */
    void print(const char* text, int number);

    /**
     * Sweeps up the motor speed. Used by menuCommandSweepMotor()
     * 
     * param delay_between_changes_millis: time between a speed change in milliseconds.
     */
    void sweepSpeedUpwards(int delay_between_changes_millis, MotorController * motor);
    /**
     * Sweeps down the motor speed. Used by menuCommandSweepMotor()
     * 
     * param delay_between_changes_millis: time between a speed change in milliseconds.
     */
    void sweepSpeedDownwards(int delay_between_changes_millis, MotorController * motor);
    /**
     * Makes the CLI wait by blinking the internal LED.
     */
    void waitBlinking();
};

#endif