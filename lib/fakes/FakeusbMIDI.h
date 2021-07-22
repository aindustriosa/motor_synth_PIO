#ifndef FakeEEPROM_h
#define FakeEEPROM_h

#include <inttypes.h>

class usb_midi_class
{
public:
  // Message type names for compatibility with Arduino MIDI library 4.3.1
  enum MidiType
  {
    InvalidType = 0x00,          // For notifying errors
    NoteOff = 0x80,              // Note Off
    NoteOn = 0x90,               // Note On
    AfterTouchPoly = 0xA0,       // Polyphonic AfterTouch
    ControlChange = 0xB0,        // Control Change / Channel Mode
    ProgramChange = 0xC0,        // Program Change
    AfterTouchChannel = 0xD0,    // Channel (monophonic) AfterTouch
    PitchBend = 0xE0,            // Pitch Bend
    SystemExclusive = 0xF0,      // System Exclusive
    TimeCodeQuarterFrame = 0xF1, // System Common - MIDI Time Code Quarter Frame
    SongPosition = 0xF2,         // System Common - Song Position Pointer
    SongSelect = 0xF3,           // System Common - Song Select
    TuneRequest = 0xF6,          // System Common - Tune Request
    Clock = 0xF8,                // System Real Time - Timing Clock
    Start = 0xFA,                // System Real Time - Start
    Continue = 0xFB,             // System Real Time - Continue
    Stop = 0xFC,                 // System Real Time - Stop
    ActiveSensing = 0xFE,        // System Real Time - Active Sensing
    SystemReset = 0xFF,          // System Real Time - System Reset
  };
  void begin(void) {}
  void end(void) {}
  void sendNoteOff(uint8_t note, uint8_t velocity, uint8_t channel, uint8_t cable = 0) {}
  void sendNoteOn(uint8_t note, uint8_t velocity, uint8_t channel, uint8_t cable = 0) {}
  void sendPolyPressure(uint8_t note, uint8_t pressure, uint8_t channel, uint8_t cable = 0) {}
  void sendAfterTouchPoly(uint8_t note, uint8_t pressure, uint8_t channel, uint8_t cable = 0) {}
  void sendControlChange(uint8_t control, uint8_t value, uint8_t channel, uint8_t cable = 0) {}
  void sendProgramChange(uint8_t program, uint8_t channel, uint8_t cable = 0) {}
  void sendAfterTouch(uint8_t pressure, uint8_t channel, uint8_t cable = 0) {}
  void sendPitchBend(int value, uint8_t channel, uint8_t cable = 0) {}
  void sendSysEx(uint32_t length, const uint8_t *data, bool hasTerm = false, uint8_t cable = 0) {}
  void sendRealTime(uint8_t type, uint8_t cable = 0)
  {
  }
  void sendTimeCodeQuarterFrame(uint8_t type, uint8_t value, uint8_t cable = 0)
  {
  }
  void sendSongPosition(uint16_t beats, uint8_t cable = 0)
  {
  }
  void sendSongSelect(uint8_t song, uint8_t cable = 0)
  {
  }
  void sendTuneRequest(uint8_t cable = 0)
  {
  }
  void beginRpn(uint16_t number, uint8_t channel, uint8_t cable = 0)
  {
  }
  void sendRpnValue(uint16_t value, uint8_t channel, uint8_t cable = 0)
  {
  }
  void sendRpnIncrement(uint8_t amount, uint8_t channel, uint8_t cable = 0)
  {
  }
  void sendRpnDecrement(uint8_t amount, uint8_t channel, uint8_t cable = 0)
  {
  }
  void endRpn(uint8_t channel, uint8_t cable = 0)
  {
  }
  void beginNrpn(uint16_t number, uint8_t channel, uint8_t cable = 0)
  {
  }
  void sendNrpnValue(uint16_t value, uint8_t channel, uint8_t cable = 0)
  {
  }
  void sendNrpnIncrement(uint8_t amount, uint8_t channel, uint8_t cable = 0)
  {
  }
  void sendNrpnDecrement(uint8_t amount, uint8_t channel, uint8_t cable = 0)
  {
  }
  void endNrpn(uint8_t channel, uint8_t cable = 0)
  {
  }
  void send(uint8_t type, uint8_t data1, uint8_t data2, uint8_t channel, uint8_t cable)
  {
  }
  void send_now(void)
  {
  }
  uint8_t analog2velocity(uint16_t val, uint8_t range);
  bool read(uint8_t channel = 0)
  {
  }
  uint8_t getType(void)
  {
    return 0;
  }
  uint8_t getCable(void)
  {
    return 0;
  }
  uint8_t getChannel(void)
  {
    return 0;
  }
  uint8_t getData1(void)
  {
    return 0;
  }
  uint8_t getData2(void)
  {
    return 0;
  }
  uint8_t *getSysExArray(void)
  {
    return 0;
  }
  uint16_t getSysExArrayLength(void)
  {
    return 0;
  }

  void setHandleNoteOff(void (*fptr)(uint8_t channel, uint8_t note, uint8_t velocity))
  {
  }
  void setHandleNoteOn(void (*fptr)(uint8_t channel, uint8_t note, uint8_t velocity))
  {
  }
  void setHandleVelocityChange(void (*fptr)(uint8_t channel, uint8_t note, uint8_t velocity))
  {
  }
  void setHandleAfterTouchPoly(void (*fptr)(uint8_t channel, uint8_t note, uint8_t pressure))
  {
  }
  void setHandleControlChange(void (*fptr)(uint8_t channel, uint8_t control, uint8_t value))
  {
  }
  void setHandleProgramChange(void (*fptr)(uint8_t channel, uint8_t program))
  {
  }
  void setHandleAfterTouch(void (*fptr)(uint8_t channel, uint8_t pressure))
  {
  }
  void setHandleAfterTouchChannel(void (*fptr)(uint8_t channel, uint8_t pressure))
  {
  }
  void setHandlePitchChange(void (*fptr)(uint8_t channel, int pitch))
  {
  }
  void setHandleSysEx(void (*fptr)(const uint8_t *data, uint16_t length, bool complete))
  {
  }
  void setHandleSystemExclusive(void (*fptr)(const uint8_t *data, uint16_t length, bool complete))
  {
  }
  void setHandleSystemExclusive(void (*fptr)(uint8_t *data, unsigned int size))
  {
  }
  void setHandleTimeCodeQuarterFrame(void (*fptr)(uint8_t data))
  {
  }
  void setHandleSongPosition(void (*fptr)(uint16_t beats))
  {
  }
  void setHandleSongSelect(void (*fptr)(uint8_t songnumber))
  {
  }
  void setHandleTuneRequest(void (*fptr)(void))
  {
  }
  void setHandleClock(void (*fptr)(void))
  {
  }
  void setHandleStart(void (*fptr)(void))
  {
  }
  void setHandleContinue(void (*fptr)(void))
  {
  }
  void setHandleStop(void (*fptr)(void))
  {
  }
  void setHandleActiveSensing(void (*fptr)(void))
  {
  }
  void setHandleSystemReset(void (*fptr)(void))
  {
  }
  void setHandleRealTimeSystem(void (*fptr)(uint8_t realtimebyte)){};
};

usb_midi_class usbMIDI;

#endif