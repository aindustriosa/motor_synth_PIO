// Monophonic synth that performs audio output by changing a motor velocity.

#ifndef _MOTOR_SYNTH_MOTOR_SYNTH_H_
#define _MOTOR_SYNTH_MOTOR_SYNTH_H_

#include "../data_model/synth_event/synth_event.h"
#include "../motor_controller/motor_controller.h"

class MotorSynth
{
public:
    void setup(int motor_control_pin);
    void processEvent(SynthEvent * event);
    /**
     * Prints current note_to_velocity table with note names
     */
    void printTunning();

private:
    MotorController motor;
#define EVENTS_STACK_SIZE 10
    // Notes pressed and not released yet.
    SynthEvent eventsStack[EVENTS_STACK_SIZE];
    void printStack();
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
    void removeNoteOnEventInStack(SynthEvent * event);

    void update_note_to_velocity();

#define NOTE_TO_VELOCITY_SIZE 128

    int note_to_velocity[NOTE_TO_VELOCITY_SIZE] = {
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0
    };
};


#endif