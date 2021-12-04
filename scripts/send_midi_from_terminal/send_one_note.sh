#!/bin/bash
# Send a G4 for a second

source ./lib_send_midi.sh  # contains functions for sending MIDI events

noteOnOff_1_sec $G4   # sending a G4 for a second
