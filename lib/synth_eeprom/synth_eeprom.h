/** Synth EEPROM: permanent storage for the Synth
 * 
 * Currently it maintains the SynthMotor tunning.
 */


#ifndef _MOTOR_SYNTH_EEPROM_H_
#define _MOTOR_SYNTH_EEPROM_H_

#ifdef NATIVE
#include "FakeEEPROM.h"
#else
#include <EEPROM.h>
#endif // NATIVE

#include "../motor_synth/motor_synth.h"

class SynthEEPROM
{
public:

    void setup();

    /**
     * returns true if the EEPROM data related to the motor synth is dirty.
     */
    bool isSynthMotorDataDirty();

    /**
     * .Marks that the EEPROM data related to the motor synth is not dirty.
     */
    void cleanSynthMotorDataDirtyByte();

    /**
     * .Marks that the EEPROM data related to the motor synth is dirty.
     */
    void uncleanSynthMotorDataDirtyByte();

    /**
     * returns the stored velocity for the given synth motor note
     * or -1 if the note is out of bounds
     */
    int16_t getSynthMotorVelocity(int note);

    /**
     * updates the stored velocity for the given synth motor note.
     * if the note is out of bounds does nothing.
     */
    void setSynthMotorVelocity(int note, int16_t velocity);

    void printSynthMotorData();


private:

    /// EEPROM Mapping:
    // A byte that indicates that the values in MotorNoteToVelocityAddress
    // have been initialized
    const int motorNoteToVelocityIsInitializedAddress = 0;
    // Where the noteToVelocity configuration is placed
    const int motorNoteToVelocityAddress = 1;
    const int motorNoteToVelocitySize = NOTE_TO_VELOCITY_SIZE;
    
    // If MotorNoteToVelocityIsInitializedAddress contains this number it means that,
    // the data in MotorNoteToVelocityAddress has been initialized
    const uint8_t motorNoteToVelocityIsInitializedMagicNumber = 58;
};

#endif // _MOTOR_SYNTH_EEPROM_H_
