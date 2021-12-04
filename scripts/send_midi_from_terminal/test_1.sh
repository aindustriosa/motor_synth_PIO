#!/bin/bash
# plays a full scale for each motor in polyphonic mode

source ./lib_send_midi.sh  # contains functions for sending MIDI events

function play_scale_4 {
    noteOnOff_sec $C4 2 ; terminal_beep
    noteOnOff_sec $D4 2 ; terminal_beep
    noteOnOff_sec $E4 2 ; terminal_beep
    noteOnOff_sec $F4 2 ; terminal_beep
    noteOnOff_sec $G4 2 ; terminal_beep
    noteOnOff_sec $A4 2 ; terminal_beep
    noteOnOff_sec $B4 2 ; terminal_beep
}

function play_scale_5 {
    noteOnOff_sec $C5 2 ; terminal_beep
    noteOnOff_sec $D5 2 ; terminal_beep
    noteOnOff_sec $E5 2 ; terminal_beep
    noteOnOff_sec $F5 2 ; terminal_beep
    noteOnOff_sec $G5 2 ; terminal_beep
    noteOnOff_sec $A5 2 ; terminal_beep
    noteOnOff_sec $B5 2 ; terminal_beep
}

function activate_1_motor {
    noteOn 2
}

function deactivate_1_motor {
    noteOff 2
}


function activate_2_motor {
    noteOn 2 ; noteOn 3
}

function deactivate_2_motor {
    noteOff 2  ; noteOff 3
}


function activate_3_motor {
    noteOn 2 ; noteOn 3 ; noteOn 4
}

function deactivate_3_motor {
    noteOff 2  ; noteOff 3 ; noteOff 4
}


play_scale_5
sleep 2; terminal_beep

activate_1_motor
sleep 2; terminal_beep
play_scale_5
sleep 2; terminal_beep
deactivate_1_motor

activate_2_motor
sleep 2; terminal_beep
play_scale_5
sleep 2; terminal_beep
deactivate_2_motor

activate_3_motor
sleep 2; terminal_beep
play_scale_5
sleep 2; terminal_beep
deactivate_3_motor

