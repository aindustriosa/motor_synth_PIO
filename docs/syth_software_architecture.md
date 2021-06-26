

src/main.cpp has some global instances:
 - a Cli (lib/cli/cli.h) instance (cli)
 - a BLink instance (used by Cli)

cli has internal objects, global to the object:
- Blink
- SynthEEPROM
- MidiInterface

cli instantiates objects in per function scope:
- SynthEvent
- MotorController


cli configures Serial at setup()