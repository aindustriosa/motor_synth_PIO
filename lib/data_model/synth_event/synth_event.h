// Data model for a synth event

// A synth event is an event that changes the behaviour of a synth.
// A synth event is commonly produced by an user interface, for example
// the MIDI interface when a note is pressed, and consumed by a synth like a
// SerialSynth that outputs the event on the serial interface or the MotorSynth
// that updates the speed of a motor.

#ifndef _MOTOR_SYNTH_SYNTH_EVENT_H_
#define _MOTOR_SYNTH_SYNTH_EVENT_H_

#include <Arduino.h> // needed for "byte" definition

// Copied from usb_midi.h
// Message type names for compatibility with Arduino MIDI library 4.3.1
enum SynthEventType
{
    InvalidType = 0x00,          // For notifying errors
    NoteOff = 0x80,              // Note Off
    NoteOn = 0x90,               // Note On
    AfterTouchPoly = 0xA0,       // Polyphonic AfterTouch
    ControlChange = 0xB0,        // Control Change / Channel Mode
    ProgramChange = 0xC0,        // Program Change
    AfterTouchChannel = 0xD0,    // Channel (monophonic) AfterTouch
    PitchBend = 0xE0,            // Pitch Bend
    SystemExclusive = 0xF0,      // System Exclusive
    TimeCodeQuarterFrame = 0xF1, // System Common - MIDI Time Code Quarter Frame
    SongPosition = 0xF2,         // System Common - Song Position Pointer
    SongSelect = 0xF3,           // System Common - Song Select
    TuneRequest = 0xF6,          // System Common - Tune Request
    Clock = 0xF8,                // System Real Time - Timing Clock
    Start = 0xFA,                // System Real Time - Start
    Continue = 0xFB,             // System Real Time - Continue
    Stop = 0xFC,                 // System Real Time - Stop
    ActiveSensing = 0xFE,        // System Real Time - Active Sensing
    SystemReset = 0xFF,          // System Real Time - System Reset
};

class SynthEvent
{
public:
    /**
     * Prints on serial port a string representation of the event.
     */
    void print();
    /**
     * Copyes the contents of this event into the given event.
     * 
     * The given event's memory must be already reserved. 
     */
    void copyInto(SynthEvent * event);

    void setType(SynthEventType type);
    SynthEventType getType();

    void setChannel(byte channel);
    byte getChannel();

    void setData1(byte data1);
    byte getData1();
    void setNote(byte note);
    byte getNote();
    void setControl(byte control);
    byte getControl();
    void setProgram(byte program);
    byte getProgram();
    void setPressure(byte pressure);
    byte getPressure();

    void setData2(byte data2);
    byte getData2();
    void setNoteVelocity(byte velocity);
    byte getNoteVelocity();
    void setControlValue(byte value);
    byte getControlValue();

    void setPichBend(int16_t pitch);
    int16_t getPitchBend();

private:
    SynthEventType type = SynthEventType::InvalidType;
    byte channel = 1;
    byte data1 = 0;
    byte data2 = 0;
};

#endif