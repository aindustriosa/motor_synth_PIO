// Monophonic synth that performs audio output by changing a motor velocity.

#ifndef _MOTOR_SYNTH_MOTOR_SYNTH_H_
#define _MOTOR_SYNTH_MOTOR_SYNTH_H_

#include "../data_model/synth_event/synth_event.h"
#include "../motor_controller/motor_controller.h"
#include "../serial/serial.h"

class MotorSynth
{
public:
    void setup(motor_synth::MotorController* motor, 
                motor_synth::SerialIO* serial );
    void setup(motor_synth::MotorController* motor, 
                int events_stack_size, 
                motor_synth::SerialIO* serial );
    void processEvent(SynthEvent *event);
    /**
     * Prints current note_to_velocity table with note names
     */
    void printTunning();

    /**
     * Copies into event the noteOn event being played now.
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
     * Updates the motor velocity by reading the notes stack and the current tunning.
     * 
     * Use this if you have changed the sound of a note if the note is being played and
     * the tunning has changed.
     */
    void updateSound();

private:
    motor_synth::MotorController* motor = nullptr;
    motor_synth::SerialIO* serialIO = nullptr;
    
#define EVENTS_STACK_MAX_SIZE 10
    int events_stack_size = EVENTS_STACK_MAX_SIZE;
    // Notes pressed and not released yet.
    SynthEvent eventsStack[EVENTS_STACK_MAX_SIZE];
    
    // The Synth only plays the note in this index. If -1 there are no notes in the stack.
    int eventsStackIndex = -1;

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

#define NOTE_TO_VELOCITY_SIZE 128
#define DRONE_VELOCITY 160 // With 160 the motor spins

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
    937, 945, 953, 960, 968, 976, 984, 992    };
};

#endif