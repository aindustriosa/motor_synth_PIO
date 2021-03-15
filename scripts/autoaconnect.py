#!/usr/bin/python3

"""
Connects all the output MIDI devices to the Teensy Motor Synth
Some code has been borrowed from https://github.com/fralalonde/autoroute/blob/master/autoroute
This script uses aconnect (alsa-utils package)

Usage: python3 autoaconnect.py

Automatic patch instructions (tested in raspbian):
1. copy this file to /usr/bin/autoaconnect.py
2. sudo chmod +x /usr/bin/autoaconnect.py
3. sudo nano /etc/udev/rules.d/77-motor_synth.rules
And copy one of these rules
# Not working: shoud work when a new MIDI device is connected
#ACTION=="bind", SUBSYSTEM=="snd_seq", DRIVERS=="snd-usb-audio", RUN+="/usr/bin/autoaconnect.py"
# Works: Launch rule when the Teensy is connected
#ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="0489", RUN="/usr/bin/autoaconnect.py"
# Works: Launch rule when any USB device is connected (just in case :P)
SUBSYSTEM=="usb", ACTION=="add", ENV{DEVTYPE}=="usb_device", RUN="/usr/bin/autoaconnect.py"
"""

import re
import subprocess

device_regex = re.compile("^client (\d+):")
port_name_regex = re.compile("^\s+(\d+)\s+'(.+)'")
ports = {}

ignore = {'Timer', 'Announce', 'Midi Through Port-0'}

motor_synth_name = 'Teensy MIDI MIDI 1'


def load_ports():
    aconnect = subprocess.Popen(['aconnect', '-o'], stdout=subprocess.PIPE)
    device_no = -1
    for line in aconnect.stdout:
        line = line.decode('utf-8')
        device_match = device_regex.match(line)
        if device_match:
            device_no = device_match.group(1)
            continue
        port_match = port_name_regex.match(line)
        if port_match:
            port_name = port_match.group(2).strip()
            if port_name not in ignore:
                ports[port_name] = device_no + ":" + port_match.group(1)


load_ports()
print("Found ports:")
print(ports)

if motor_synth_name not in ports.keys():
    print("Motor Synth not connected")
    exit(-1)

any_connection_made = False

for midi_ouput_name in ports.keys():
    if midi_ouput_name == motor_synth_name:
        continue

    print(f'Connecting {midi_ouput_name} [{ports[midi_ouput_name]}] to {motor_synth_name} [{ports[motor_synth_name]}]')
    subprocess.Popen(['aconnect', ports[midi_ouput_name], ports[motor_synth_name]])
    any_connection_made = True

if any_connection_made is False:
    print("No connection has been made")
