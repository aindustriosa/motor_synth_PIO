
#include <Arduino.h>
#include "midi_interface.h"


// Helper that translates MIDI inputs to SynthEvent
void processMIDI(SynthEvent * event);

void MidiInterface::setup()
{
    Serial.println("Setting up MidiInterface");
}

int MidiInterface::getSynthEvent(SynthEvent *event)
{
    int res = -1;
    if (usbMIDI.read())
    {
        processMIDI(event);
        res = 1;
    }
    return res;
}

void processMIDI(SynthEvent *event)
{
    event->setChannel(usbMIDI.getChannel()); // which MIDI channel, 1-16
    event->setData1(usbMIDI.getData1());     // first data byte of message, 0-127
    event->setData2(usbMIDI.getData2());     // second data byte of message, 0-127

    byte type = usbMIDI.getType(); // which MIDI message, 128-255
    switch (type)
    {
    case usbMIDI.NoteOff:
        event->setType(SynthEventType::NoteOff);
        break;

    case usbMIDI.NoteOn:
        event->setType(SynthEventType::NoteOn);
        break;

    case usbMIDI.AfterTouchPoly:
        event->setType(SynthEventType::AfterTouchPoly);
        break;

    case usbMIDI.ControlChange:
        event->setType(SynthEventType::ControlChange);
        break;

    case usbMIDI.ProgramChange:
        event->setType(SynthEventType::ProgramChange);
        break;

    case usbMIDI.AfterTouchChannel:
        event->setType(SynthEventType::AfterTouchChannel);
        break;

    case usbMIDI.PitchBend:
        event->setType(SynthEventType::PitchBend);
        break;

    case usbMIDI.TimeCodeQuarterFrame:
        event->setType(SynthEventType::TimeCodeQuarterFrame);
        break;

    case usbMIDI.SongPosition:
        event->setType(SynthEventType::SongPosition);
        break;

    case usbMIDI.SongSelect:
        event->setType(SynthEventType::SongSelect);
        break;

    case usbMIDI.TuneRequest:
        event->setType(SynthEventType::TuneRequest);
        break;

    case usbMIDI.Clock:
        event->setType(SynthEventType::Clock);
        break;

    case usbMIDI.Start:
        event->setType(SynthEventType::Start);
        break;

    case usbMIDI.Continue:
        event->setType(SynthEventType::Continue);
        break;

    case usbMIDI.Stop:
        event->setType(SynthEventType::Stop);
        break;

    case usbMIDI.ActiveSensing:
        event->setType(SynthEventType::ActiveSensing);
        break;

    case usbMIDI.SystemReset:
        event->setType(SynthEventType::SystemReset);
        break;

    default:
        event->setType(SynthEventType::InvalidType);
    }
}
