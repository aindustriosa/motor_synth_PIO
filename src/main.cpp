#include <Arduino.h>
#include "cli.h"
#include "servo_motor_controller.h"
#include "arduino_serial.h"

#define DO_NOT_PRINT
#ifndef DO_NOT_PRINT
motor_synth::ArduinoSerialIO serial;
#else
motor_synth::ArduinoSerialIODoNotPrint serial;
#endif

CLI global_cli;
Blink blink;                 // board's led controller
SynthEEPROM synthEEPROM;     // permanent storage
motor_synth::MidiInterface midiInterface; // MIDI I/O
motor_synth::MotorSynth motorSynth; // Manages the notes stack and motor/s velocity state/s

// For each motor, we have a ServoMotorController and a control pin number
#define NUMBER_OF_MOTORS 4
motor_synth::ServoMotorController *motors[NUMBER_OF_MOTORS];
// Pins possitions: https://www.pjrc.com/store/teensy32.html
// Available pins: https://www.pjrc.com/teensy/td_libs_Servo.html
int motor_control_pins[NUMBER_OF_MOTORS] = {9, 10, 3, 4};


void setup()
{
  Serial.begin(115200);
  serial.println("Setup: blink");
  blink.setup();
  synthEEPROM.setup();
  midiInterface.setup();
  for (int i = 0; i < NUMBER_OF_MOTORS; i++)
  {
    motor_synth::ServoMotorController* motor = new motor_synth::ServoMotorController();
    motors[i] = motor;
    motors[i]->setup(motor_control_pins[i]);
  }
  motorSynth.setup((motor_synth::MotorController**)motors, NUMBER_OF_MOTORS, &serial);
  motorSynth.setIsMonophonic(true);
  global_cli.setup(&blink, &synthEEPROM, &midiInterface, &serial, &motorSynth);
}

void loop()
{
  // Uncomment the MIDI based user interface
  //cli.loop_midi();
  // Uncomment for a Serial based UI
  //cli.loop_serial();

  // Or run an specific application:
  global_cli.menuCommandMonophonicSynthTunning();
}
