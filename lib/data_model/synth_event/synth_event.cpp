#include "synth_event.h"

namespace motor_synth
{

void SynthEvent::print(motor_synth::SerialIO *serial)
{
    switch (type)
    {
    case SynthEventType::NoteOff:
        serial->print("Note Off, ch=");
        serial->print(channel, DEC);
        serial->print(", note=");
        serial->print(data1, DEC);
        serial->print(", velocity=");
        serial->println(data2, DEC);
        break;

    case SynthEventType::NoteOn:
        serial->print("Note On, ch=");
        serial->print(channel, DEC);
        serial->print(", note=");
        serial->print(data1, DEC);
        serial->print(", velocity=");
        serial->println(data2, DEC);
        break;

    case SynthEventType::AfterTouchPoly:
        serial->print("AfterTouch Change, ch=");
        serial->print(channel, DEC);
        serial->print(", note=");
        serial->print(data1, DEC);
        serial->print(", velocity=");
        serial->println(data2, DEC);
        break;

    case SynthEventType::ControlChange:
        serial->print("Control Change, ch=");
        serial->print(channel, DEC);
        serial->print(", control=");
        serial->print(data1, DEC);
        serial->print(", value=");
        serial->println(data2, DEC);
        break;

    case SynthEventType::ProgramChange:
        serial->print("Program Change, ch=");
        serial->print(channel, DEC);
        serial->print(", program=");
        serial->println(data1, DEC);
        break;

    case SynthEventType::AfterTouchChannel:
        serial->print("After Touch, ch=");
        serial->print(channel, DEC);
        serial->print(", pressure=");
        serial->println(data1, DEC);
        break;

    case SynthEventType::PitchBend:
        serial->print("Pitch Change, ch=");
        serial->print(channel, DEC);
        serial->print(", pitch=");
        serial->println(data1 + data2 * 128, DEC);
        break;

    case SynthEventType::TimeCodeQuarterFrame:
        serial->print("TimeCode, index=");
        serial->print(data1 >> 4, DEC);
        serial->print(", digit=");
        serial->println(data1 + data2 * 128, DEC);
        break;

    case SynthEventType::SongPosition:
        serial->print("Song Position, beat=");
        serial->println(data1 + data2 * 128, DEC);
        break;

    case SynthEventType::SongSelect:
        serial->print("Song Select, song=");
        serial->println(data1, DEC);
        break;

    case SynthEventType::TuneRequest:
        serial->println("Tune Request");
        break;

    case SynthEventType::Clock:
        //serial->println("Clock");
        break;

    case SynthEventType::Start:
        serial->println("Start");
        break;

    case SynthEventType::Continue:
        serial->println("Continue");
        break;

    case SynthEventType::Stop:
        serial->println("Stop");
        break;

    case SynthEventType::ActiveSensing:
        serial->println("Active Sensing");
        break;

    case SynthEventType::SystemReset:
        serial->println("System Reset");
        break;

    default:
        serial->println("Unknown MIDI message type");
    }
}

void SynthEvent::copyInto(SynthEvent *event)
{
    event->type = this->type;
    event->channel = this->channel;
    event->data1 = this->data1;
    event->data2 = this->data2;
}

void SynthEvent::setType(SynthEventType type)
{
    this->type = type;
}
SynthEventType SynthEvent::getType()
{
    return this->type;
}

void SynthEvent::setChannel(byte channel)
{
    this->channel = channel;
}
byte SynthEvent::getChannel()
{
    return this->channel;
}

void SynthEvent::setData1(byte data1)
{
    this->data1 = data1;
}
byte SynthEvent::getData1()
{
    return this->data1;
}
void SynthEvent::setNote(byte note)
{
    this->data1 = note;
}
byte SynthEvent::getNote()
{
    return this->data1;
}
void SynthEvent::setControl(byte control)
{
    this->data1 = control;
}
byte SynthEvent::getControl()
{
    return this->data1;
}
void SynthEvent::setProgram(byte program)
{
    this->data1 = program;
}
byte SynthEvent::getProgram()
{
    return this->data1;
}
void SynthEvent::setPressure(byte pressure)
{
    this->data1 = pressure;
}
byte SynthEvent::getPressure()
{
    return this->data1;
}

void SynthEvent::setData2(byte data2)
{
    this->data2 = data2;
}
byte SynthEvent::getData2()
{
    return this->data2;
}
void SynthEvent::setNoteVelocity(byte velocity)
{
    this->data2 = data2;
}
byte SynthEvent::getNoteVelocity()
{
    return this->data2;
}
void SynthEvent::setControlValue(byte value)
{
    this->data2 = data2;
}
byte SynthEvent::getControlValue()
{
    return this->data2;
}

void SynthEvent::setPichBend(int16_t pitch)
{
    this->data1 = (byte)pitch;
    this->data2 = (byte)(pitch / 128);
}
int16_t SynthEvent::getPitchBend()
{
    return (int16_t)(this->data1 + this->data2 * 128);
}
} // motor_synth