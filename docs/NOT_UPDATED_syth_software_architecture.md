

src/main.cpp has some global instances:
 - a Cli (lib/cli/cli.h) instance (cli)
 - a BLink instance (used by Cli)
 - a SynthEEPROM instance (used by Cli)

cli has some internal objects, global to the object:
- Blink
- SynthEEPROM
- MidiInterface

cli instantiates objects in per function scope:
- SynthEvent
- MotorController


main configures Serial at setup()