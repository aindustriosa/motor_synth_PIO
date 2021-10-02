# Just send something from terminal
```sh
$ amidi -l # List available devices
Dir Device    Name
IO  hw:1,0,0  Teensy MIDI MIDI 1

$ amidi -p hw:1,0,0 -S '90 68 64'  # Send a NoteOn
$ amidi -p hw:1,0,0 -S '80 68 64'  # Send a NoteOff
```

# Using an external controller (MIDI keyboard)

```sh
$ aconnect -i # List available MIDI devices
client 0: 'System' [type=kernel]
    0 'Timer           '
    1 'Announce        '
client 14: 'Midi Through' [type=kernel]
    0 'Midi Through Port-0'
client 20: 'Teensy MIDI' [type=kernel,card=1]
    0 'Teensy MIDI MIDI 1'
client 24: 'nanoKEY Studio' [type=kernel,card=2]
    0 'nanoKEY Studio MIDI 1'


$ aconnect 24:0 20:0  # Connect nanokey to Teensy
$ aconnect -d 24:0 20:0 # Disconnect
```

If you don't have a physical MIDI keyboard at hand, you can use a simple software keyboard like vkeybd in the same way.


# MIDI Sniffer
You can use a virtual device that prints received events and wire the keyboard output to the Teensy and the virtual device. This can be useful to debug some keyboards that could send unexpected events.

To create a virtual device:
```sh
$ amidi -p virtual -d # This blocks this terminal

$ aconnect -l
client 0: 'System' [type=kernel]
    0 'Timer           '
    1 'Announce        '
client 14: 'Midi Through' [type=kernel]
    0 'Midi Through Port-0'
client 20: 'Teensy MIDI' [type=kernel,card=1]
    0 'Teensy MIDI MIDI 1'
	Connected From: 24:0
client 24: 'nanoKEY Studio' [type=kernel,card=2]
    0 'nanoKEY Studio MIDI 1'
	Connecting To: 20:0
client 128: 'Client-128' [type=user,pid=139677]
    0 'Virtual RawMIDI '
```

Now you can make the wiring as before:
```sh
$ aconnect 24:0 128:0
```
