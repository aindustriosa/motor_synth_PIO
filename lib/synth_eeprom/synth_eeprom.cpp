
#include "synth_eeprom.h"
#include <Arduino.h>

void SynthEEPROM::setup()
{
    Serial.println("Setup: SynthEEPROM");
}

bool SynthEEPROM::isSynthMotorDataDirty()
{
    uint8_t value = EEPROM.read(this->motorNoteToVelocityIsInitializedAddress);
    return value != this->motorNoteToVelocityIsInitializedMagicNumber;
}

void SynthEEPROM::cleanSynthMotorDataDirtyByte()
{
    if (this->isSynthMotorDataDirty())
    {
        EEPROM.write(this->motorNoteToVelocityIsInitializedAddress,
                     this->motorNoteToVelocityIsInitializedMagicNumber);
    }
}

void SynthEEPROM::uncleanSynthMotorDataDirtyByte()
{
    if (!this->isSynthMotorDataDirty())
    {
        EEPROM.write(this->motorNoteToVelocityIsInitializedAddress,
                     this->motorNoteToVelocityIsInitializedMagicNumber + 1);
    }
}

int16_t SynthEEPROM::getSynthMotorVelocity(int note)
{
    if ((note >= this->motorNoteToVelocitySize) || (note < 0))
    {
        return -1;
    }
    int address = note * sizeof(int16_t) + this->motorNoteToVelocityAddress;

    int16_t velocity;
    int8_t *v = (int8_t *)&velocity;
    for (unsigned int i = 0; i < sizeof(int16_t); i++)
    {
        v[i] = EEPROM.read(address + i);
    }
    return velocity;
}

void SynthEEPROM::setSynthMotorVelocity(int note, int16_t velocity)
{
    if ((note >= this->motorNoteToVelocitySize) || (note < 0))
    {
        return;
    }
    int address = note * sizeof(int16_t) + this->motorNoteToVelocityAddress;

    int8_t *v = (int8_t *)&velocity;
    for (unsigned int i = 0; i < sizeof(int16_t); i++)
    {
        EEPROM.update(address + i, v[i]);
    }
}

void SynthEEPROM::printSynthMotorData()
{
    Serial.println("EEPROM Motor values:");
    for (int note = 0; note < this->motorNoteToVelocitySize; note++)
    {
        while (!Serial)
        {
            ; // wait for serial port to be availabl. Ugly hack
        }
        Serial.printf("Note ");
        Serial.print(note, DEC);
        Serial.printf("  velocity ");
        Serial.print(this->getSynthMotorVelocity(note), DEC);
        Serial.println();
    }
    Serial.printf("  Magic number: ");
    Serial.print(EEPROM.read(this->motorNoteToVelocityIsInitializedAddress), DEC);
    Serial.println();
}