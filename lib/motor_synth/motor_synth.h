// Monophonic synth that performs audio output by changing a motor velocity.

#ifndef _MOTOR_SYNTH_MOTOR_SYNTH_H_
#define _MOTOR_SYNTH_MOTOR_SYNTH_H_

#include "../data_model/synth_event/synth_event.h"
#include "../motor_controller/motor_controller.h"
#include "../motor_controller/servo_motor_controller.h"
#include "../serial/serial.h"

namespace motor_synth
{
    class MotorSynth
    {
    public:
        motor_synth::MotorController **motors;
        int motors_len = 0;
        static const int MAX_NUMBER_MOTORS = 8;

        void setup(
            motor_synth::MotorController **motors,
            int motors_len,
            motor_synth::SerialIO *serial);
        void setup(
            motor_synth::MotorController **motors,
            int motors_len,
            int events_stack_size,
            motor_synth::SerialIO *serial);

        /**
         * Updates notes stack and motors given the new event.
         */
        void processEvent(SynthEvent *event);
        /**
         * Updates notes stack and motors given the new event.
         */
        void updateStackWithNewEvent(SynthEvent *event);
        /**
         * Updates motor speeds using the current notes stack.
         */
        void applyStackToMotors();
        /**
         * Prints current note_to_velocity table with note names
         */
        void printTunning();

        /**
         * Copies into event the noteOn event being played now. In case of many notes played
         * (because many motors are configured) returns the top of the stack (newest note).
         * 
         * Returns a SynthEventType::InvalidType if no note is being played
         * 
         * event's memory must be already reserved 
         */
        void getCurrentlyPlayedNote(SynthEvent *event);

        /**
         * Updates the velocity assigned to a note.
         */
        void tune_note(int note, int16_t velocity);

        void printNoteToVelocity();

        /**
         * Returns -1 if note is not in the noteToVelocity range
         */
        int16_t getNoteVelocity(int note);

        void printStack();

        /**
         * Updates the motor/s velocity/ies by reading the notes stack and the current tunning.
         * 
         * Use this if the note is being played and the tunning has changed.
         */
        void updateSound();

        int get_events_stack_size() { return events_stack_size; }
        SynthEvent *get_eventsStack() { return eventsStack; }
        int get_eventsStackIndex() { return eventsStackIndex; }
        SynthEvent *getEventPerMotor() { return this->eventPerMotor; }
        int getEventsStackIndex() { return this->eventsStackIndex; }
        void setIsMonophonic(bool isMonophonic) { this->isMonophonic = isMonophonic ;}

        ~MotorSynth() { destroyEventPerMotor(); }

    private:
        bool isMonophonic = false;
        motor_synth::SerialIO *serialIO;

#define EVENTS_STACK_MAX_SIZE 10
        int events_stack_size = EVENTS_STACK_MAX_SIZE;
        // Notes pressed and not released yet.
        SynthEvent eventsStack[EVENTS_STACK_MAX_SIZE];

        // Top of the notes stack index. If -1 there are no notes in the stack.
        int eventsStackIndex = -1;

        // The event (note) that each motor is playing currently. If the event type is not
        // NoteOn the motor is playing nothing.
        SynthEvent *eventPerMotor;

        // Sets to SynthEventType::InvalidType an entry in eventPerMotor per motor.
        void createEventPerMotor();

        // Removes allocated memory for eventPerMotor
        void destroyEventPerMotor();

        /**
         * Liberates the last slot in eventsStack, removing the older event and moving
         * the rest of events.
         * 
         * Updates eventsStackIndex, that points to the more recent note in the stack. 
         */
        void removeOldestEventInStack();
        /**
         * Liberates event from eventsStack, moving the rest of events.
         * 
         * Updates eventsStackIndex, that points to the more recent note in the stack. 
         */
        void removeNoteOnEventInStack(SynthEvent *event);

        /**
         * Sets a naive tunning based in a geometric ramp.
         */
        void update_note_to_velocity();

        void applyStackToMotorsMonophonic();
        void applyStackToMotorsPolyphonic();
        /**
         * applyStackToMotorsPolyphonic uses some RAM for a couple of arrays.
         * They are created here so they are not reserved at each call 
         */
        // Pointers to notes in the eventsStack that have to be played by a motor
        // in this loop but aren't yet.
        SynthEvent *notesWaitingForMotor[EVENTS_STACK_MAX_SIZE];
        int notesWaitingForMotorIndex = -1;
        // one boolean per motor. If true, the motor is currently playing a note
        // that has to be played. If false, the motor is playing a note that has
        // to be updated.
        bool isMotorAvailableForChange[MAX_NUMBER_MOTORS];

        // These methods are useful for polyphonic motor assignment algorithm
        void clearNotesWaitingForMotor();
        void clearIsMotorAvailableForChange();
        /**
         * Returns the index (in this->motors) of the motor playing the given note, -1 if 
         * no motor is playing it. If more than one motor is playing it, it returns the 
         * first one.
         */
        int whichMotorIsPlayingThisNote(SynthEvent* noteEvent);


        void applyEventToMotor(motor_synth::SynthEvent * event, int motor_index);

#define NOTE_TO_VELOCITY_SIZE 128
#define DRONE_VELOCITY 0 // With 160 the motor spins

        int16_t noteToVelocity[NOTE_TO_VELOCITY_SIZE] = {
            0, 7, 15, 23, 31, 39, 46, 54, 62, 70,
            78, 85, 93, 101, 109, 117, 125, 132, 140, 148,
            156, 180, 171, 187, 187, 195, 203, 210, 218, 226,
            234, 242, 250, 257, 265, 273, 281, 289, 296, 304,
            312, 320, 328, 335, 343, 351, 359, 367, 238, 382,
            250, 398, 260, 264, 270, 278, 277, 285, 293, 305,
            319, 330, 341, 354, 366, 379, 391, 408, 425, 441,
            462, 487, 501, 570, 578, 585, 593, 601, 609, 617,
            625, 632, 640, 648, 656, 664, 671, 679, 687, 695,
            703, 710, 718, 726, 734, 742, 750, 757, 765, 773,
            781, 789, 796, 804, 812, 820, 828, 835, 843, 851,
            859, 867, 875, 882, 890, 898, 906, 914, 921, 929,
            937, 945, 953, 960, 968, 976, 984, 992};
    };
}

#endif