
#include <Arduino.h>
#include "motor_synth.h"

namespace motor_synth
{
    void MotorSynth::update_note_to_velocity()
    {
        int max_speed = this->motors[0].getMaxSpeed();
        for (int i = 0; i < NOTE_TO_VELOCITY_SIZE; i++)
        {
            noteToVelocity[i] = map(
                i,
                0, NOTE_TO_VELOCITY_SIZE,
                0, max_speed);
        }
    }

    void MotorSynth::printNoteToVelocity()
    {
        this->serialIO->println("int noteToVelocity[NOTE_TO_VELOCITY_SIZE] = {");
        for (int i = 0; i < NOTE_TO_VELOCITY_SIZE; i++)
        {
            if (i % 10 == 0)
            {
                this->serialIO->print("    ");
            }
            this->serialIO->print(noteToVelocity[i], DEC);
            if (i < NOTE_TO_VELOCITY_SIZE - 1)
            {
                if (i % 10 == 9)
                {
                    this->serialIO->println(",");
                }
                else
                {
                    this->serialIO->print(", ");
                }
            }
        }
        this->serialIO->println("    };");
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
            this->serialIO->print(i, DEC);
            this->serialIO->print(" - ");
            this->serialIO->print(note);
            this->serialIO->print(": ");
            this->serialIO->print(noteToVelocity[i], DEC);
            this->serialIO->println();
        }
    }

    void MotorSynth::setup(
        motor_synth::MotorController *motors,
        int motors_len,
        motor_synth::SerialIO *serial)
    {
        this->motors = motors;
        this->motors_len = motors_len;
        this->events_stack_size = EVENTS_STACK_MAX_SIZE;
        this->serialIO = serial;
        // update_note_to_velocity();
        //this->serialIO->println("MotorSynth setup completed");
    }

    void MotorSynth::setup(
        motor_synth::MotorController *motors,
        int motors_len,
        int events_stack_size,
        motor_synth::SerialIO *serial)
    {
        this->motors = motors;
        this->motors_len = motors_len;
        this->serialIO = serial;
        if ((events_stack_size >= EVENTS_STACK_MAX_SIZE) || (events_stack_size < 1))
        {
            this->events_stack_size = EVENTS_STACK_MAX_SIZE;
        }
        else
        {
            this->events_stack_size = events_stack_size;
        }
        //update_note_to_velocity();
        this->serialIO->println("MotorSynth setup completed");
    }

    void MotorSynth::processEvent(SynthEvent *event)
    {
        switch (event->getType())
        {
        case SynthEventType::NoteOn:
            // First of all, we play the note
            this->motors[0].setSpeed(noteToVelocity[event->getNote()]);
            // Now, we add the note to the stack
            if (this->eventsStackIndex >= this->events_stack_size - 1)
            {
                // We need to make room for the new event
                removeOldestEventInStack();
            }

            this->eventsStackIndex = this->eventsStackIndex + 1;
            event->copyInto(&(this->eventsStack[this->eventsStackIndex]));
            break;

        case SynthEventType::NoteOff:
            // This noteOff could be any of the notes in the stack (or maybe it is not in the stack),
            // we need to remove the respective noteOn in the stack and play the more recent note.
            removeNoteOnEventInStack(event);
            // Now, we play the more recent note in the stack
            if (this->eventsStackIndex < 0)
            {
                // No notes in the stack
                this->motors[0].setSpeed(DRONE_VELOCITY);
            }
            else
            {
                this->motors[0].setSpeed(noteToVelocity[this->eventsStack[this->eventsStackIndex].getNote()]);
            }
            break;

        default:
            break;
        }
    }

    void MotorSynth::updateSound()
    {
        this->motors[0].setSpeed(noteToVelocity[this->eventsStack[this->eventsStackIndex].getNote()]);
    }

    void MotorSynth::removeOldestEventInStack()
    {
        if (this->eventsStackIndex >= this->events_stack_size)
        {
            this->eventsStackIndex = this->events_stack_size - 1;
        }

        const int currentIndex = this->eventsStackIndex;
        for (int i = 0; i < currentIndex; i++)
        {
            this->eventsStack[i + 1].copyInto(&(this->eventsStack[i]));
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
            this->eventsStack[i + 1].copyInto(&(this->eventsStack[i]));
        }

        this->eventsStackIndex = this->eventsStackIndex - 1;
    }

    void MotorSynth::printStack()
    {
        if (this->eventsStackIndex >= this->events_stack_size)
        {
            this->eventsStackIndex = this->events_stack_size - 1;
        }

        const int currentIndex = this->eventsStackIndex;
        this->serialIO->println("--- Current Stack ---");
        for (int i = 0; i <= currentIndex; i++)
        {
            this->serialIO->print("- ");
            this->serialIO->print(i, DEC);
            this->serialIO->print(" - ");
            this->eventsStack[i].print();
        }
        this->serialIO->println("--------------------");
    }

    void MotorSynth::tune_note(int note, int16_t velocity)
    {
        if (note >= NOTE_TO_VELOCITY_SIZE)
        {
            return;
        }
        noteToVelocity[note] = velocity;
    }

    int16_t MotorSynth::getNoteVelocity(int note)
    {
        if (note >= NOTE_TO_VELOCITY_SIZE)
        {
            return -1;
        }
        return noteToVelocity[note];
    }

    void MotorSynth::getCurrentlyPlayedNote(SynthEvent *event)
    {
        if (this->eventsStackIndex >= this->events_stack_size)
        {
            this->eventsStackIndex = this->events_stack_size - 1;
        }
        if (this->eventsStackIndex < 0)
        {
            event->setType(SynthEventType::InvalidType);
            return;
        }

        this->eventsStack[this->eventsStackIndex].copyInto(event);
    }
}