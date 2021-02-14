#include <Arduino.h>
#include "cli.h"

CLI cli;

void setup()
{
  cli.setup();
}

void loop()
{
  cli.loop();
}
