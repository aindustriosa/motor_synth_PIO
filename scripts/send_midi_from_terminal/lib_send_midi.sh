#!/bin/bash
# contains functions for sending MIDI events

TEENSY_PORT=$(amidi -l | grep "Teensy MIDI" -m 1 | awk '{print $2}')

# MIDI CODES
G9=127
Fsus9=126
Gb9=126
F9=125
E9=124
Dsus9=123
Eb9=123
D9=122
Db9=121
Csus9=121
C9=120
B8=119
Bb8=118
Asus8=118
A8=117
Ab8=116
Gsus8=116
G8=115
Gb8=114
Fsus8=114
F8=113
E8=112
Eb8=111
Dsus8=111
D8=110
Db8=109
Csus8=109
C8=108
B7=107
Bb7=106
Asus7=106
A7=105
Ab7=104
Gsus7=104
G7=103
Gb7=102
Fsus7=102
F7=101
E7=100
Eb7=99
Dsus7=99
D7=98
Db7=97
Csus7=97
C7=96
B6=95
Bb6=94
Asus6=94
A6=93
Ab6=92
Gsus6=92
G6=91
Gb6=90
Fsus6=90
F6=89
E6=88
Eb6=87
Dsus6=87
D6=86
Db6=85
Csus6=85
C6=84
B5=83
Bb5=82
Asus5=82
A5=81
Ab5=80
Gsus5=80
G5=79
Gb5=78
Fsus5=78
F5=77
E5=76
Eb5=75
Dsus5=75
D5=74
Db5=73
Csus5=73
C5=72
B4=71
Bb4=70
Asus4=70
A4=69
Ab4=68
Gsus4=68
G4=67
Gb4=66
Fsus4=66
F4=65
E4=64
Eb4=63
Dsus4=63
D4=62
Db4=61
Csus4=61
C4=60
B3=59
Bb3=58
Asus3=58
A3=57
Ab3=56
Gsus3=56
G3=55
Gb3=54
Fsus3=54
F3=53
E3=52
Eb3=51
Dsus3=51
D3=50
Db3=49
Csus3=49
C3=48
B2=47
Bb2=46
Asus2=46
A2=45
Ab2=44
Gsus2=44
G2=43
Gb2=42
Fsus2=42
F2=41
E2=40
Eb2=39
Dsus2=39
D2=38
Db2=37
Csus2=37
C2=36
B1=35
Bb1=34
Asus1=34
A1=33
Ab1=32
Gsus1=32
G1=31
Gb1=30
Fsus1=30
F1=29
E1=28
Eb1=27
Dsus1=27
D1=26
Db1=25
Csus1=25
C1=24
B0=23
Bb0=22
Asus0=22
A0=21


NoteOn=90
NoteOff=80


# Send a MIDI event
# USAGE: send_midi "$MIDI_EVENT_TYPE $MIDI_DATA_1 $MIDI_DATA_2"
function send_midi {
    amidi -p ${TEENSY_PORT} -S $1
}

# Send a noteOn
# USAGE: noteOn $NOTE_HEX_CODE
function noteOn {
    send_midi "$NoteOn $1 64"
}

# Send a noteOff
# USAGE: noteOff $NOTE_HEX_CODE
function noteOff {
    send_midi "$NoteOff $1 64"
}

# Send a noteOn, wait a second, send a noteOff for a given note
# USAGE: noteOnOff_sec $NOTE_HEX_CODE $SECONDS
function noteOnOff_sec {
    send_midi "$NoteOn $1 64"
    sleep $2
    send_midi "$NoteOff $1 64"
}

# Send a noteOn, wait a second, send a noteOff for a given note
# USAGE: noteOnOff_1_sec $NOTE_HEX_CODE
function noteOnOff_1_sec {
    noteOnOff_sec "$1 $2"
}


function terminal_beep {
    echo -ne '\a'
}
