#include "synth_event.h"

void SynthEvent::print()
{
    switch (type)
    {
    case SynthEventType::NoteOff:
        Serial.print("Note Off, ch=");
        Serial.print(channel, DEC);
        Serial.print(", note=");
        Serial.print(data1, DEC);
        Serial.print(", velocity=");
        Serial.println(data2, DEC);
        break;

    case SynthEventType::NoteOn:
        Serial.print("Note On, ch=");
        Serial.print(channel, DEC);
        Serial.print(", note=");
        Serial.print(data1, DEC);
        Serial.print(", velocity=");
        Serial.println(data2, DEC);
        break;

    case SynthEventType::AfterTouchPoly:
        Serial.print("AfterTouch Change, ch=");
        Serial.print(channel, DEC);
        Serial.print(", note=");
        Serial.print(data1, DEC);
        Serial.print(", velocity=");
        Serial.println(data2, DEC);
        break;

    case SynthEventType::ControlChange:
        Serial.print("Control Change, ch=");
        Serial.print(channel, DEC);
        Serial.print(", control=");
        Serial.print(data1, DEC);
        Serial.print(", value=");
        Serial.println(data2, DEC);
        break;

    case SynthEventType::ProgramChange:
        Serial.print("Program Change, ch=");
        Serial.print(channel, DEC);
        Serial.print(", program=");
        Serial.println(data1, DEC);
        break;

    case SynthEventType::AfterTouchChannel:
        Serial.print("After Touch, ch=");
        Serial.print(channel, DEC);
        Serial.print(", pressure=");
        Serial.println(data1, DEC);
        break;

    case SynthEventType::PitchBend:
        Serial.print("Pitch Change, ch=");
        Serial.print(channel, DEC);
        Serial.print(", pitch=");
        Serial.println(data1 + data2 * 128, DEC);
        break;

    case SynthEventType::TimeCodeQuarterFrame:
        Serial.print("TimeCode, index=");
        Serial.print(data1 >> 4, DEC);
        Serial.print(", digit=");
        Serial.println(data1 & 15, DEC);
        break;

    case SynthEventType::SongPosition:
        Serial.print("Song Position, beat=");
        Serial.println(data1 + data2 * 128);
        break;

    case SynthEventType::SongSelect:
        Serial.print("Song Select, song=");
        Serial.println(data1, DEC);
        break;

    case SynthEventType::TuneRequest:
        Serial.println("Tune Request");
        break;

    case SynthEventType::Clock:
        //Serial.println("Clock");
        break;

    case SynthEventType::Start:
        Serial.println("Start");
        break;

    case SynthEventType::Continue:
        Serial.println("Continue");
        break;

    case SynthEventType::Stop:
        Serial.println("Stop");
        break;

    case SynthEventType::ActiveSensing:
        Serial.println("Active Sensing");
        break;

    case SynthEventType::SystemReset:
        Serial.println("System Reset");
        break;

    default:
        Serial.println("Unknown MIDI message type");
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
