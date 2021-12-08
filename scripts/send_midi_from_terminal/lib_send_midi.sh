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

lib_send_midi_help_texts=("# # # Available functions in lib_send_midi # # #")


lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_send_midi\e[0m \$MIDI_EVENT_TYPE \$MIDI_DATA_1 \$MIDI_DATA_2:
        Send a MIDI event."
)
function lib_send_midi_send_midi {
    amidi -p ${TEENSY_PORT} -S $1
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_noteOn\e[0m \$NOTE_HEX_CODE: Send a MIDI noteOn event."
)
function lib_send_midi_noteOn {
    lib_send_midi_send_midi "$NoteOn $1 64"
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_noteOff\e[0m \$NOTE_HEX_CODE: Send a MIDI noteOff event."
)
function lib_send_midi_noteOff {
    lib_send_midi_send_midi "$NoteOff $1 64"
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_noteOnOff_sec\e[0m \$NOTE_HEX_CODE \$SECONDS: Send a MIDI noteOn,
        wait a number of seconds, send a noteOff for a given note."
)
function lib_send_midi_noteOnOff_sec {
    lib_send_midi_send_midi "$NoteOn $1 64"
    sleep $2
    lib_send_midi_send_midi "$NoteOff $1 64"
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_noteOnOff_1_sec\e[0m \$NOTE_HEX_CODE: Send a MIDI noteOn,
        wait a second, send a noteOff for a given note."
)
function lib_send_midi_noteOnOff_1_sec {
    lib_send_midi_noteOnOff_sec "$1 $2"
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_terminal_beep\e[0m: Send a terminal beep."
)
function lib_send_midi_terminal_beep {
    echo -ne '\a'
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_play_scale_4\e[0m: Play the 4th scale."
)
function lib_send_midi_play_scale_4 {
    for note in $C4 $D4 $E4 $F4 $G4 $A4 $B4
    do
        lib_send_midi_noteOnOff_sec $note 2
        lib_send_midi_terminal_beep
    done
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_play_scale_5\e[0m: Play the 5th scale."
)
function lib_send_midi_play_scale_5 {
    for note in $C5 $D5 $E5 $F5 $G5 $A5 $B5
    do
        lib_send_midi_noteOnOff_sec $note 2
        lib_send_midi_terminal_beep
    done
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_activate_1_motor\e[0m: Send noteOn with a very low note so
        motor 1 keeps running."
)
function lib_send_midi_activate_1_motor {
    lib_send_midi_noteOn 2
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_deactivate_1_motor\e[0m: Undo lib_send_midi_activate_1_motor."
)
function lib_send_midi_deactivate_1_motor {
    lib_send_midi_noteOff 2
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_activate_2_motor\e[0m: Send two noteOn with very low notes so
        motor 1 and 2 keep running."
)
function lib_send_midi_activate_2_motor {
    lib_send_midi_activate_1_motor
    lib_send_midi_noteOn 3
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_deactivate_2_motor\e[0m: Undo lib_send_midi_activate_2_motor."
)
function lib_send_midi_deactivate_2_motor {
    lib_send_midi_deactivate_1_motor
    lib_send_midi_noteOff 3
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_activate_3_motor\e[0m: Send three noteOn with very low notes so
        motor 1, 2 and 3 keep running."
)
function lib_send_midi_activate_3_motor {
    lib_send_midi_activate_2_motor
    lib_send_midi_noteOn 4
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_deactivate_3_motor\e[0m: Undo lib_send_midi_activate_3_motor."
)
function lib_send_midi_deactivate_3_motor {
    lib_send_midi_deactivate_2_motor
    lib_send_midi_noteOff 4
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_activate_3_motor\e[0m: Send four noteOn with very low notes so
        motor 1, 2, 3 and 4 keep running."
)
function lib_send_midi_activate_4_motor {
    lib_send_midi_activate_3_motor
    lib_send_midi_noteOn 5
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_deactivate_4_motor\e[0m: Undo lib_send_midi_activate_4_motor."
)
function lib_send_midi_deactivate_4_motor {
    lib_send_midi_deactivate_3_motor
    lib_send_midi_noteOff 5
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_remove_all_midi_connections\e[0m: Remove all midi patches 
        (that where made by autoaconnect, for example)."
)
function lib_send_midi_remove_all_midi_connections {
    aconnect --removeall
}

lib_send_midi_help_texts+=(
"\e[31mlib_send_midi_print_help\e[0m: Print this help."
)
function lib_send_midi_print_help {
    for line in "${lib_send_midi_help_texts[@]}"
    do
        echo -e "${line}"
    done
}

