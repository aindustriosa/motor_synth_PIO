#include <Arduino.h>
#include "cli.h"
#include "arduino_serial.h"

CLI cli;
Blink blink; // board's led controller
SynthEEPROM synthEEPROM; // permanent storage
MidiInterface midiInterface;
motor_synth::ArduinoSerialIO serial;

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
