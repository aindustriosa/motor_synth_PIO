
#include <Arduino.h>
#include "motor_synth.h"



void MotorSynth::update_note_to_velocity(){
    int max_speed = this->motor.getMaxSpeed();
    for (int i = 0; i < NOTE_TO_VELOCITY_SIZE; i++){
        note_to_velocity[i] = map(i, 
        0, NOTE_TO_VELOCITY_SIZE, 
        0, max_speed);
    }
}

void numberToNote(int value, char * note, int len_note){
    for (int i = 0; i < len_note; i++){
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
        note[index+1] = '#';
        index = index + 2;
        break;
    
    case 2:
        note[index] = 'D';
        index = index + 1;
        break;
    
    case 3:
        note[index] = 'D';
        note[index+1] = '#';
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
        note[index+1] = '#';
        index = index + 2;
        break;

   case 7:
        note[index] = 'G';
        index = index + 1;
        break;
    
    case 8:
        note[index] = 'G';
        note[index+1] = '#';
        index = index + 2;
        break;
    
    case 9:
        note[index] = 'A';
        index = index + 1;
        break;
    
    case 10:
        note[index] = 'A';
        note[index+1] = '#';
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

    int octave = (value / 12) -1;
    if (octave == -1){
        note[index] = '-';
        note[index+1] = '1';
        index = index + 2;
    } else {
        note[index] = '0' + octave;
        index = index + 1;
    }
}

void MotorSynth::printTunning(){
    const int len_note = 5;
    char note[len_note];
    for (int i = 0; i < NOTE_TO_VELOCITY_SIZE; i++){
        numberToNote(i, note, len_note);
        Serial.print(note);
        Serial.print(": ");
        Serial.print(note_to_velocity[i], DEC);
        Serial.println();
    }
}

void MotorSynth::setup(int motor_control_pin){
    this->motor.setup(motor_control_pin);
    update_note_to_velocity();
    Serial.println("MotorSynth setup completed");
}

void MotorSynth::processEvent(SynthEvent * event){

    switch (event->getType())
    {
    case SynthEventType::NoteOn:
        motor.setSpeed(note_to_velocity[event->getNote()]);
        break;

    case SynthEventType::NoteOff:
        motor.setSpeed(0);
        break;
    
    default:
        break;
    }
}