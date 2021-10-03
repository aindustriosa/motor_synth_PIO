// Captures MIDI events from the USB port
// Convert a MIDI event to a SynthEvent
// Returns the SynthEvent that has arrived.

// Uses Teensy's USBMidi library:  https://www.pjrc.com/teensy/td_midi.html

#ifndef _MOTOR_SYNTH_MIDI_INTERFACE_H_
#define _MOTOR_SYNTH_MIDI_INTERFACE_H_

#include "../data_model/synth_event/synth_event.h"

namespace motor_synth
{

class MidiInterface
{
public:
    /**
     * MidiInterface configuration.
     * 
     * Use up to setup the CLI.
     */
    void setup();
    /**
     * Get a received MIDI event as a SynthEvent
     * 
     * event's memory must be allocated beforehand.
     * 
     * Returns a negative number if no event available. 
     */
    int getSynthEvent(motor_synth::SynthEvent * event);

private:
};
} // motor_synth

#endif