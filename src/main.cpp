#include <Arduino.h>
#include "cli.h"
#include "arduino_serial.h"

#define DO_NOT_PRINT

CLI cli;
Blink blink; // board's led controller
SynthEEPROM synthEEPROM; // permanent storage
MidiInterface midiInterface;
#ifndef DO_NOT_PRINT
motor_synth::ArduinoSerialIO serial;
#else
motor_synth::ArduinoSerialIODoNotPrint serial;
#endif

void setup()
{
  Serial.begin(115200);
  serial.println("Setup: blink");
  blink.setup();
  synthEEPROM.setup();
  midiInterface.setup();
  cli.setup(&blink, &synthEEPROM, &midiInterface, &serial);
}

void loop()
{
  // Uncomment the MIDI based user interface
  //cli.loop_midi();
  // Uncomment for a Serial based UI
  //cli.loop_serial();

  // Or run an specific application:
  cli.menuCommandMonophonicSynthTunning(cli.MOTOR_CONTROL_PIN);
}
