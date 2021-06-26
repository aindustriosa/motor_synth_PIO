#include <Arduino.h>
#include "cli.h"

CLI cli;
Blink blink; // board's led controller
SynthEEPROM synthEEPROM; // permanent storage

void setup()
{
  Serial.begin(115200);
  Serial.println("Setup: blink");
  blink.setup();
  synthEEPROM.setup();
  cli.setup(&blink, &synthEEPROM);
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
