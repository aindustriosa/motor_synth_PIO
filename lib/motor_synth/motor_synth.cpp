
#include <Arduino.h>
#include "motor_synth.h"

namespace motor_synth
{
    void MotorSynth::update_note_to_velocity()
    {
        int max_speed = this->motors[0]->getMaxSpeed();
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
        motor_synth::MotorController **motors,
        int motors_len,
        motor_synth::SerialIO *serial)
    {
        this->motors = motors;
        this->motors_len = motors_len;
        this->events_stack_size = EVENTS_STACK_MAX_SIZE;
        this->serialIO = serial;
        this->createEventPerMotor();
        // update_note_to_velocity();
        this->serialIO->println("MotorSynth setup completed");
    }

    void MotorSynth::setup(
        motor_synth::MotorController **motors,
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
        this->createEventPerMotor();
        //update_note_to_velocity();
        this->serialIO->println("MotorSynth setup completed");
    }

    void MotorSynth::createEventPerMotor()
    {
        this->eventPerMotor = new SynthEvent[this->motors_len];
        for (int i = 0; i < this->motors_len; i++)
        {
            this->eventPerMotor[i].setType(SynthEventType::InvalidType);
        }
    }

    void MotorSynth::destroyEventPerMotor()
    {
        delete[] this->eventPerMotor;
    }

    void MotorSynth::processEvent(SynthEvent *event)
    {
        updateStackWithNewEvent(event);
        applyStackToMotors();
    }

    void MotorSynth::updateStackWithNewEvent(SynthEvent *event)
    {
        switch (event->getType())
        {
        case SynthEventType::NoteOn:
            // In case we receive two NoteON with no NoteOff
            removeNoteOnEventInStack(event);
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
            // we need to remove the respective noteOn in the stack.
            removeNoteOnEventInStack(event);
            break;

        default:
            break;
        }
    }

    void MotorSynth::applyStackToMotors()
    {
        if (this->eventsStackIndex < 0) // No notes in the stack
        {
            for (int motor_index = 0; motor_index < this->motors_len; motor_index++)
            {
                this->motors[motor_index]->setSpeed(DRONE_VELOCITY);
                this->eventPerMotor[motor_index].setType(SynthEventType::InvalidType);
            }
        }
        else
        {
            if (this->isMonophonic)
            {
                this->applyStackToMotorsMonophonic();
            }
            else
            {
                this->applyStackToMotorsPolyphonic();
            }
        }
    }

    // You can assume at least one note in the stack
    void MotorSynth::applyStackToMotorsMonophonic()
    {
        for (int motor_index = 0; motor_index < this->motors_len; motor_index++)
        {
            this->applyEventToMotor(
                &(this->eventsStack[this->eventsStackIndex]),
                motor_index);
        }
    }

    // You can assume at least one note in the stack
    void MotorSynth::applyStackToMotorsPolyphonic()
    {
        // Stage 0; reset variables
        this->clearNotesWaitingForMotor();
        this->clearIsMotorAvailableForChange();

        // Stage 1; update notesWaitingForMotorIndex and isMotorWellAssigned.
        int maxAssignableEvents = motors_len; // We can manage as many different notes as motors we have
        for (int i = this->eventsStackIndex; i >= 0; i--)
        {
            int motorPlayingThisNote = this->whichMotorIsPlayingThisNote(this->eventsStack + i);
            if (motorPlayingThisNote == -1)
            {
                // No motor playing this note, we need to play it.
                this->notesWaitingForMotorIndex = this->notesWaitingForMotorIndex + 1;
                this->notesWaitingForMotor[this->notesWaitingForMotorIndex] = this->eventsStack + i;
            }
            else
            {
                // motor i is playing the note, this motor cannot change note
                this->isMotorAvailableForChange[motorPlayingThisNote] = false;
            }
            maxAssignableEvents = maxAssignableEvents - 1;
            if (maxAssignableEvents == 0)
            {
                break;
            }
        }

        // Stage 2; apply the notes not played to the avaliable motors
        for (int i_note = this->notesWaitingForMotorIndex; i_note >= 0; i_note--)
        {
            for (int i_motor = 0; i_motor < this->motors_len; i_motor++)
            {
                if (this->isMotorAvailableForChange[i_motor] == true)
                {
                    this->applyEventToMotor(this->eventsStack + i_note, i_motor);
                    this->isMotorAvailableForChange[i_motor] = false;
                }
            }
        }
    }

    void MotorSynth::clearNotesWaitingForMotor()
    {
        for (int i = 0; i < EVENTS_STACK_MAX_SIZE; i++)
        {
            this->notesWaitingForMotor[i] = NULL;
        }
        this->notesWaitingForMotorIndex = -1;
    }

    // Clear means that is available for change (is free to play a note)
    void MotorSynth::clearIsMotorAvailableForChange()
    {
        for (int i = 0; i < MAX_NUMBER_MOTORS; i++)
        {
            this->isMotorAvailableForChange[i] = true;
        }
    }

    int MotorSynth::whichMotorIsPlayingThisNote(SynthEvent *noteEvent)
    {
        int result = -1;
        int objectiveVelocity = noteToVelocity[noteEvent->getNote()];

        for (int i = 0; i < this->motors_len; i++)
        {
            if (this->motors[i]->getSpeed() == objectiveVelocity)
            {
                result = i;
                break;
            }
        }

        return result;
    }

    void MotorSynth::applyEventToMotor(motor_synth::SynthEvent *event, int motor_index)
    {
        this->motors[motor_index]->setSpeed(this->noteToVelocity[event->getNote()]);
        event->copyInto(&(this->eventPerMotor[motor_index]));
    }

    void MotorSynth::updateSound()
    {
        this->applyStackToMotors();
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
            this->eventsStack[i].print(this->serialIO);
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