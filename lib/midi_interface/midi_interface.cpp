
#include <Arduino.h>
#include "midi_interface.h"

#ifdef NATIVE
#include "FakeusbMIDI.h"
#endif // NATIVE

namespace motor_synth
{

// Helper that translates MIDI inputs to SynthEvent
void processMIDI(motor_synth::SynthEvent * event);

void MidiInterface::setup()
{
    // TODO: Use arduino_serial
    Serial.println("Setting up MidiInterface");
}

int MidiInterface::getSynthEvent(motor_synth::SynthEvent *event)
{
    int res = -1;
    if (usbMIDI.read())
    {
        processMIDI(event);
        res = 1;
    }
    return res;
}

void processMIDI(motor_synth::SynthEvent *event)
{
    event->setChannel(usbMIDI.getChannel()); // which MIDI channel, 1-16
    event->setData1(usbMIDI.getData1());     // first data byte of message, 0-127
    event->setData2(usbMIDI.getData2());     // second data byte of message, 0-127

    byte type = usbMIDI.getType(); // which MIDI message, 128-255
    switch (type)
    {
    case usbMIDI.NoteOff:
        event->setType(motor_synth::SynthEventType::NoteOff);
        break;

    case usbMIDI.NoteOn:
        event->setType(motor_synth::SynthEventType::NoteOn);
        break;

    case usbMIDI.AfterTouchPoly:
        event->setType(motor_synth::SynthEventType::AfterTouchPoly);
        break;

    case usbMIDI.ControlChange:
        event->setType(motor_synth::SynthEventType::ControlChange);
        break;

    case usbMIDI.ProgramChange:
        event->setType(motor_synth::SynthEventType::ProgramChange);
        break;

    case usbMIDI.AfterTouchChannel:
        event->setType(motor_synth::SynthEventType::AfterTouchChannel);
        break;

    case usbMIDI.PitchBend:
        event->setType(motor_synth::SynthEventType::PitchBend);
        break;

    case usbMIDI.TimeCodeQuarterFrame:
        event->setType(motor_synth::SynthEventType::TimeCodeQuarterFrame);
        break;

    case usbMIDI.SongPosition:
        event->setType(motor_synth::SynthEventType::SongPosition);
        break;

    case usbMIDI.SongSelect:
        event->setType(motor_synth::SynthEventType::SongSelect);
        break;

    case usbMIDI.TuneRequest:
        event->setType(motor_synth::SynthEventType::TuneRequest);
        break;

    case usbMIDI.Clock:
        event->setType(motor_synth::SynthEventType::Clock);
        break;

    case usbMIDI.Start:
        event->setType(motor_synth::SynthEventType::Start);
        break;

    case usbMIDI.Continue:
        event->setType(motor_synth::SynthEventType::Continue);
        break;

    case usbMIDI.Stop:
        event->setType(motor_synth::SynthEventType::Stop);
        break;

    case usbMIDI.ActiveSensing:
        event->setType(motor_synth::SynthEventType::ActiveSensing);
        break;

    case usbMIDI.SystemReset:
        event->setType(motor_synth::SynthEventType::SystemReset);
        break;

    default:
        event->setType(motor_synth::SynthEventType::InvalidType);
    }
}
}