#include <Arduino.h>
#include "cli.h"

CLI cli;
Blink blink; // board's led controller
SynthEEPROM synthEEPROM; // permanent storage
MidiInterface midiInterface;

void setup()
{
  Serial.begin(115200);
  Serial.println("Setup: blink");
  blink.setup();
  synthEEPROM.setup();
  midiInterface.setup();
  cli.setup(&blink, &synthEEPROM, &midiInterface);
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
