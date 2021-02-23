
#include "cli.h"

void CLI::setup()
{
  Serial.begin(115200);
  print("Setup: blink");
  blink.setup();
  print("Setup: end");
}

void CLI::loop()
{
  print("##############################################");
  print("Motor Synth: Menu");
  print("1: Change motor speed using this serial port");
  print("2: Sweep motor speed");
  print("3: MIDI in to serial port");
  print("4: Monophonic synth");
  print("5: Monophonic synth tunning");
  waitBlinking();

  int value = getSerialPositiveValue();
  if (value == 1)
    menuCommandChangeMotorOnUserSerialInput(MOTOR_CONTROL_PIN);
  if (value == 2)
    menuCommandSweepMotor(MOTOR_CONTROL_PIN);
  if (value == 3)
    menuCommandMidiInterfaceTest();
  if (value == 4)
    menuCommandMonophonicSynth(MOTOR_CONTROL_PIN);
  if (value == 5)
    menuCommandMonophonicSynthTunning(MOTOR_CONTROL_PIN);

  if (value < 0)
    waitBlinking();
}

void CLI::waitBlinking()
{
  for (int i = 0; i < 20; i++)
  {
    blink.blink();
  }
}

int CLI::getSerialPositiveValue()
{
  int res = -1;
  if (Serial.available())
  {
    res = Serial.readStringUntil('\n').toInt();
  }
  return res;
}

void CLI::print(const char *text)
{
  Serial.println(text);
}

void CLI::print(const char *text, int number)
{
  Serial.print(text);
  Serial.println(number);
}

void CLI::menuCommandChangeMotorOnUserSerialInput(int motor_control_pin)
{
  MotorController motor;
  print("Setup: motor on pin", motor_control_pin);
  motor.setup(motor_control_pin);

  print("Changing motor speed using this serial port:\nEnter a number to change the speed");
  print("Values from 0 to ", motor.getMaxSpeed());
  print("Reset the board to go back to the main menu.");

  waitBlinking();

  while (1)
  {
    int value = -1;
    while (value < 0)
    {
      value = getSerialPositiveValue();
      if (value < 0)
        waitBlinking();
    }

    motor.setSpeed(value);
    print("Speed:", value);
    blink.toggle();
  }
}

void CLI::menuCommandSweepMotor(int motor_control_pin)
{
  MotorController motor;
  print("Setup: motor on pin ", motor_control_pin);
  motor.setup(motor_control_pin);

  print("Sweeping motor speed:");
  print("Please enter a value for the wait millisecs between speed changes (default is 100)");
  print("Reset the board to go back to the main menu.");

  waitBlinking();

  int delay_between_changes_millis = 100;
  int value = -1;
  while (value < 0)
  {
    value = getSerialPositiveValue();
    if (value < 0)
      waitBlinking();
  }

  if (value > 0)
  {
    delay_between_changes_millis = value;
  }

  while (1)
  {
    sweepSpeedUpwards(delay_between_changes_millis, &motor);
    sweepSpeedDownwards(delay_between_changes_millis, &motor);
  }
}

void CLI::sweepSpeedUpwards(int delay_between_changes_millis, MotorController *motor)
{
  int maxSpeed = motor->getMaxSpeed();
  for (int value = 0; value <= maxSpeed; value++)
  {
    motor->setSpeed(value);
    print("Speed:", value);
    blink.toggle();
    delay(delay_between_changes_millis);
  }
}

void CLI::sweepSpeedDownwards(int delay_between_changes_millis, MotorController *motor)
{
  int maxSpeed = motor->getMaxSpeed();
  for (int value = maxSpeed; value >= 0; value--)
  {
    motor->setSpeed(value);
    print("Speed:", value);
    blink.toggle();
    delay(delay_between_changes_millis);
  }
}

void CLI::menuCommandMidiInterfaceTest()
{
  SynthEvent event = SynthEvent();
  MidiInterface midiInterface;
  midiInterface.setup();

  print("Printing out incoming MIDI events (LED toggles on new event).");
  print("Please, read the docs to get some ideas on how to send MIDI to the device.");
  print("Reset the board to go back to the menu");

  while (true)
  {
    int res = midiInterface.getSynthEvent(&event);
    if (res >= 0)
    {
      event.print();
      blink.toggle();
    }
  }
}

void CLI::menuCommandMonophonicSynth(int motor_control_pin)
{
  SynthEvent event = SynthEvent();
  MidiInterface midiInterface;
  midiInterface.setup();
  MotorSynth motorSynth;
  motorSynth.setup(motor_control_pin);

  print("Monophonic Synthesizer (LED toggles on new event).");
  print("Please, read the docs to get some ideas on how to send MIDI to the device.");
  print("Reset the board to go back to the menu");

  motorSynth.printTunning();

  while (true)
  {
    int res = midiInterface.getSynthEvent(&event);
    if (res >= 0)
    {
      motorSynth.processEvent(&event);
      event.print();
      blink.toggle();
    }
  }
}

void CLI::menuCommandMonophonicSynthTunning(int motor_control_pin)
{
  print("Monophonic Synthesizer tuner (LED toggles on new event).");
  print("Press the MIDI note that you want to tune.");
  print("Use a MIDI control (fader, potentiometer) to perform the tunning.");
  print("The tunning change is relative to the control change, if you want to tune down a note");
  print("turn all right the potentiometer before pushing the note an then go left.");
  print("Release the MIDI note to stop the tunning (other notes pressed before this will be discarded).");
  print("Please, read the docs to get some ideas on how to send MIDI to the device.");
  print("Reset the board to go back to the menu");

  SynthEvent event = SynthEvent();
  MidiInterface midiInterface;
  midiInterface.setup();
  MotorSynth motorSynth;
  // We use here a event stack size of 1 so notes don't get stacked
  motorSynth.setup(motor_control_pin, 1);

  // The note we are going to tune. InvalidType if no note active.
  SynthEvent noteBeingTunned;
  noteBeingTunned.setType(SynthEventType::InvalidType);
  int controlValue = -1;            // Last control value. -1 if no value (note has been released)
  boolean mustProcessEvent = false; // Wether noteBeingTunned must be sent to the synth to play the note
  motorSynth.printTunning();

  while (true)
  {
    int res = midiInterface.getSynthEvent(&event);

    if (res >= 0)
    {
      switch (event.getType())
      {
      case SynthEventType::NoteOn: // Update noteBeingTunned
        if (noteBeingTunned.getType() != SynthEventType::NoteOn)
        {
          event.copyInto(&noteBeingTunned);
          controlValue = -1;
          mustProcessEvent = true;
        }
        break;

      case SynthEventType::NoteOff: // Update noteBeingTunned
        if (event.getNote() == noteBeingTunned.getNote())
        {
          noteBeingTunned.setType(SynthEventType::NoteOff);
          controlValue = -1;
          mustProcessEvent = true;
          motorSynth.printNoteToVelocity();
        }
        break;

      case SynthEventType::ControlChange:
        if (noteBeingTunned.getType() != SynthEventType::NoteOn)
        {
          break;
        }
        if (controlValue > -1)
        {
          // Not the first control update, update the note tuning with the difference 
          // of control values
          int difference = event.getControlValue() - controlValue;
          int updatedVelocity = motorSynth.getNoteVelocity(noteBeingTunned.getNote()) + 
                                difference;
          motorSynth.tune_note(noteBeingTunned.getNote(), updatedVelocity);
          Serial.print("updatedVelocity ");
          Serial.println(updatedVelocity, DEC);
          mustProcessEvent = true;
        }
        controlValue = event.getControlValue();
        break;

      default:
        break;
      }
    }

    if (mustProcessEvent == true)
    {
      // Play current note with current tunning
      motorSynth.processEvent(&noteBeingTunned);
      noteBeingTunned.print();
      blink.toggle();
      mustProcessEvent = false;
    }
  }
}