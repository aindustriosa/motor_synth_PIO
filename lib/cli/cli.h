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
#include "../synth_eeprom/synth_eeprom.h"
#include "../serial/serial.h"

class CLI
{
public:
    /**
     * CLI configuration.
     * 
     * Use in the main setup to setup the CLI.
     * 
     * Parameters:
     *  - blink: pointer to an already configured Blink instance. 
     *  - synthEEPROM: pointer to an already configured SynthEEPROM instance.
     *  - midiInterface: pointer to an already configured MidiInterface instance.
     */
    void setup( Blink* blink, 
                SynthEEPROM* synthEEPROM, 
                MidiInterface* midiInterface, 
                motor_synth::SerialIO* serial,
                motor_synth::MotorSynth* motorSynth);
    /**
     * CLI loop serial version.
     * 
     * Presents the main menu and launches the user selection.
     * User selects by using the serial port.
     */
    void loop_serial();
    /**
     * CLI loop MIDI.
     * 
     * Presents the main menu and launches the user selection.
     * User selects by using MIDI control to switch menu entry and NoteOff for selection.
     */
    void loop_midi();

    void menuCommandChangeMotorOnUserSerialInput();
    void menuCommandSweepMotor();
    void menuCommandMidiInterfaceTest();
    void menuCommandMonophonicSynth();
    void menuCommandMonophonicSynthTunning();

private:
    Blink* blink = nullptr; // Board LED management
    SynthEEPROM* synthEEPROM = nullptr; // permanent storage
    MidiInterface* midiInterface = nullptr;
    motor_synth::SerialIO* serialIO = nullptr;
    motor_synth::MotorSynth* motorSynth = nullptr;

    // Main menu commands
    const int MENU_ITEMS_LEN = 5;

    // Some helpers
    /**
     * Prints on serial port the main menu highlighting the selected item.
     */
    void printMenu(int menu_item_selected);

    void printMenuCommandMonophonicSynthTunning();
    /**
     * Waits for the user to enter a positive number. Returns it.
     * Returns a negative value on failure.
     */
    int getSerialPositiveValue();
    /**
     * Sweeps up the motor speed. Used by menuCommandSweepMotor()
     * 
     * param delay_between_changes_millis: time between a speed change in milliseconds.
     */
    void sweepSpeedUpwards(int delay_between_changes_millis, motor_synth::MotorController *motor);
    /**
     * Sweeps down the motor speed. Used by menuCommandSweepMotor()
     * 
     * param delay_between_changes_millis: time between a speed change in milliseconds.
     */
    void sweepSpeedDownwards(int delay_between_changes_millis, motor_synth::MotorController *motor);
    /**
     * Makes the CLI wait by blinking the internal LED.
     */
    void waitBlinking();

    /**
     * Updates all the EEPROM motor data
     */
    void updateEEPROMMotorData(motor_synth::MotorSynth * motorSynth);
};

#endif