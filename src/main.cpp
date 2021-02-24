#include <Arduino.h>
#include "cli.h"

CLI cli;

void setup()
{
  cli.setup();
}

void loop()
{
  // Uncomment the user interface that you preffer
  cli.loop_midi();
  //cli.loop_serial();
}
