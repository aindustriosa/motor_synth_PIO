// User Interface

#ifndef _MOTOR_SYNTH_EEPROM_H_
#define _MOTOR_SYNTH_EEPROM_H_

#include <EEPROM.h>

class SynthEEPROM
{
public:

    void setup();

    /**
     * Checks the SynthMotor stored data status using the magic number.
     * Copies the data if not initialized.  
     */
    void synthMotorDataInit();

    /**
     * Updates a single note velocity (writes only if needs to)
     */
    void synthMotorDataUpdateNoteVelocity(int note, int value);

    /**
     * Get a single note velocity
     */
    int synthMotorDataGetNoteVelocity(int note);

private:

    /// EEPROM Mapping:
    // A byte that indicates that the values in MotorNoteToVelocityAddress
    // have been initialized
    const int motorNoteToVelocityIsInitializedAddress = 0;
    // Where the noteToVelocity configuration is placed
    const int motorNoteToVelocityAddress = 1;
    
    // If MotorNoteToVelocityIsInitializedAddress contains this number it means that,
    // the data in MotorNoteToVelocityAddress has been initialized
    const uint8_t motorNoteToVelocityIsInitializedMagicNumber = 56;
};

#endif // _MOTOR_SYNTH_EEPROM_H_
