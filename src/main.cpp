#include <Arduino.h>
#include "cli.h"

CLI cli;

void setup()
{
  cli.setup();
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
