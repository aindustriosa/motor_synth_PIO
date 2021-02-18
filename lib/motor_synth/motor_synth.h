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