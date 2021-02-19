
#include <Arduino.h>
#include "motor_synth.h"

void MotorSynth::update_note_to_velocity()
{
    int max_speed = this->motor.getMaxSpeed();
    for (int i = 0; i < NOTE_TO_VELOCITY_SIZE; i++)
    {
        note_to_velocity[i] = map(i,
                                  0, NOTE_TO_VELOCITY_SIZE,
                                  0, max_speed);
    }
}

void numberToNote(int value, char *note, int len_note)
{
    for (int i = 0; i < len_note; i++)
    {
        note[i] = 0;
    }
    int index = 0;

    switch (value % 12)
    {
    case 0:
        note[index] = 'C';
        index = index + 1;
        break;

    case 1:
        note[index] = 'C';
        note[index + 1] = '#';
        index = index + 2;
        break;

    case 2:
        note[index] = 'D';
        index = index + 1;
        break;

    case 3:
        note[index] = 'D';
        note[index + 1] = '#';
        index = index + 2;
        break;

    case 4:
        note[index] = 'E';
        index = index + 1;
        break;

    case 5:
        note[index] = 'F';
        index = index + 1;
        break;

    case 6:
        note[index] = 'F';
        note[index + 1] = '#';
        index = index + 2;
        break;

    case 7:
        note[index] = 'G';
        index = index + 1;
        break;

    case 8:
        note[index] = 'G';
        note[index + 1] = '#';
        index = index + 2;
        break;

    case 9:
        note[index] = 'A';
        index = index + 1;
        break;

    case 10:
        note[index] = 'A';
        note[index + 1] = '#';
        index = index + 2;
        break;

    case 11:
        note[index] = 'B';
        index = index + 1;
        break;

    default:
        note[index] = '?';
        index = index + 1;
        break;
    }

    int octave = (value / 12) - 1;
    if (octave == -1)
    {
        note[index] = '-';
        note[index + 1] = '1';
        index = index + 2;
    }
    else
    {
        note[index] = '0' + octave;
        index = index + 1;
    }
}

void MotorSynth::printTunning()
{
    const int len_note = 5;
    char note[len_note];
    for (int i = 0; i < NOTE_TO_VELOCITY_SIZE; i++)
    {
        numberToNote(i, note, len_note);
        Serial.print(note);
        Serial.print(": ");
        Serial.print(note_to_velocity[i], DEC);
        Serial.println();
    }
}

void MotorSynth::setup(int motor_control_pin)
{
    this->motor.setup(motor_control_pin);
    update_note_to_velocity();
    Serial.println("MotorSynth setup completed");
}

void MotorSynth::processEvent(SynthEvent *event)
{

    switch (event->getType())
    {
    case SynthEventType::NoteOn:
        // First of all, we play the note
        this->motor.setSpeed(note_to_velocity[event->getNote()]);
        // Now, we add the note to the stack
        if (this->eventsStackIndex >= EVENTS_STACK_SIZE - 1)
        {
            // We need to make room for the new event
            removeOldestEventInStack();
        }

        this->eventsStackIndex = this->eventsStackIndex + 1;
        this->eventsStack[this->eventsStackIndex].setChannel(event->getChannel());
        this->eventsStack[this->eventsStackIndex].setData1(event->getData1());
        this->eventsStack[this->eventsStackIndex].setData2(event->getData2());
        this->eventsStack[this->eventsStackIndex].setType(event->getType());

        printStack();
        break;

    case SynthEventType::NoteOff:
        // This noteOff could be any of the notes in the stack (or maybe it is not in the stack),
        // we need to remove the respective noteOn in the stack and play the more recent note.
        removeNoteOnEventInStack(event);
        // Now, we play the more recent note in the stack
        if (this->eventsStackIndex < 0)
        {
            // No notes in the stack
            this->motor.setSpeed(0);
        }
        else
        {
            this->motor.setSpeed(note_to_velocity[this->eventsStack[this->eventsStackIndex].getNote()]);
        }
        printStack();
        break;

    default:
        break;
    }
}

void MotorSynth::removeOldestEventInStack()
{
    if (this->eventsStackIndex >= EVENTS_STACK_SIZE)
    {
        this->eventsStackIndex = EVENTS_STACK_SIZE - 1;
    }

    const int currentIndex = this->eventsStackIndex;
    for (int i = 0; i < currentIndex; i++)
    {
        this->eventsStack[i].setChannel(this->eventsStack[i + 1].getChannel());
        this->eventsStack[i].setData1(this->eventsStack[i + 1].getData1());
        this->eventsStack[i].setData2(this->eventsStack[i + 1].getData2());
        this->eventsStack[i].setType(this->eventsStack[i + 1].getType());
    }

    this->eventsStackIndex = this->eventsStackIndex - 1;
}

void MotorSynth::removeNoteOnEventInStack(SynthEvent *event)
{
    // Note that the incoming event can be a the correspondent NoteOff event
    // (same channel and note)

    // If the note to be removed is the top of the stack, we just change the index:
    const int currentIndex = this->eventsStackIndex;
    if (this->eventsStack[currentIndex].getNote() == event->getNote())
    {
        if (this->eventsStack[currentIndex].getChannel() == event->getChannel())
        {
            this->eventsStackIndex = this->eventsStackIndex - 1;
            return;
        }
    }

    // Now we search the event in the stack.
    int remove_index = -1;
    for (int i = 0; i < currentIndex; i++)
    {
        if (this->eventsStack[i].getNote() == event->getNote())
        {
            if (this->eventsStack[i].getChannel() == event->getChannel())
            {
                remove_index = i;
                break;
            }
        }
    }
    if (remove_index == -1)
    {
        return;
    }

    // Remove event at remove_index
    for (int i = remove_index; i < currentIndex; i++)
    {
        this->eventsStack[i].setChannel(this->eventsStack[i + 1].getChannel());
        this->eventsStack[i].setData1(this->eventsStack[i + 1].getData1());
        this->eventsStack[i].setData2(this->eventsStack[i + 1].getData2());
        this->eventsStack[i].setType(this->eventsStack[i + 1].getType());
    }

    this->eventsStackIndex = this->eventsStackIndex - 1;
}

void MotorSynth::printStack()
{
    if (this->eventsStackIndex >= EVENTS_STACK_SIZE)
    {
        this->eventsStackIndex = EVENTS_STACK_SIZE - 1;
    }

    const int currentIndex = this->eventsStackIndex;
    Serial.println("--- Current Stack ---");
    for (int i = 0; i <= currentIndex; i++)
    {
        Serial.print("- ");
        Serial.print(i, DEC);
        Serial.print(" - ");
        this->eventsStack[i].print();
    }
    Serial.println("--------------------");
}