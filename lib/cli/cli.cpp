
#include "cli.h"

void CLI::setup()
{
  Serial.begin(115200);
  println("Setup: blink");
  this->blink.setup();
  this->midiInterface.setup();
  println("Setup: end");
}

void CLI::loop_serial()
{
  printMenu(-1);

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

void CLI::printMenu(int menu_item_selected)
{
  println("##############################################");
  println("Motor Synth: Menu");
  if (menu_item_selected == 0)
    print("SELECTED - ");
  println("1: Change motor speed using this serial port");
  if (menu_item_selected == 1)
    print("SELECTED - ");
  println("2: Sweep motor speed");
  if (menu_item_selected == 2)
    print("SELECTED - ");
  println("3: MIDI in to serial port");
  if (menu_item_selected == 3)
    print("SELECTED - ");
  println("4: Monophonic synth");
  if (menu_item_selected == 4)
    print("SELECTED - ");
  println("5: Monophonic synth tunning");
}

void CLI::loop_midi()
{
  int menu_item_selected = 0; // Used for loop_midi to keep the current menu selected
  SynthEvent event = SynthEvent();

  while (true)
  {
    int res = this->midiInterface.getSynthEvent(&event);
    if (res >= 0)
    {
      this->blink.toggle();
      event.print();
      if (event.getType() == SynthEventType::NoteOff)
      {
        if (menu_item_selected == 0)
          menuCommandChangeMotorOnUserSerialInput(MOTOR_CONTROL_PIN);
        if (menu_item_selected == 1)
          menuCommandSweepMotor(MOTOR_CONTROL_PIN);
        if (menu_item_selected == 2)
          menuCommandMidiInterfaceTest();
        if (menu_item_selected == 3)
          menuCommandMonophonicSynth(MOTOR_CONTROL_PIN);
        if (menu_item_selected == 4)
          menuCommandMonophonicSynthTunning(MOTOR_CONTROL_PIN);
      }

      if (event.getType() == SynthEventType::ControlChange)
      {
        menu_item_selected = (menu_item_selected + 1) % this->MENU_ITEMS_LEN;
        printMenu(menu_item_selected);
      }
    }
  }
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

void CLI::println(const char *text)
{
  Serial.println(text);
}

void CLI::print(const char *text)
{
  Serial.print(text);
}

void CLI::println(const char *text, int number)
{
  Serial.print(text);
  Serial.println(number);
}

void CLI::menuCommandChangeMotorOnUserSerialInput(int motor_control_pin)
{
  MotorController motor;
  println("Setup: motor on pin", motor_control_pin);
  motor.setup(motor_control_pin);

  println("Changing motor speed using this serial port:\nEnter a number to change the speed");
  println("Values from 0 to ", motor.getMaxSpeed());
  println("Reset the board to go back to the main menu.");

  while (1)
  {
    int value = -1;
    while (value < 0)
    {
      value = getSerialPositiveValue();
    }

    motor.setSpeed(value);
    println("Speed:", value);
    blink.toggle();
  }
}

void CLI::menuCommandSweepMotor(int motor_control_pin)
{
  MotorController motor;
  println("Setup: motor on pin ", motor_control_pin);
  motor.setup(motor_control_pin);

  println("Sweeping motor speed:");
  println("Please enter a value for the wait millisecs between speed changes (default is 100)");
  println("Reset the board to go back to the main menu.");

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
    println("Speed:", value);
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
    println("Speed:", value);
    blink.toggle();
    delay(delay_between_changes_millis);
  }
}

void CLI::menuCommandMidiInterfaceTest()
{
  println("Printing out incoming MIDI events (LED toggles on new event).");
  println("Please, read the docs to get some ideas on how to send MIDI to the device.");
  println("Reset the board to go back to the menu");

  SynthEvent event = SynthEvent();

  while (true)
  {
    int res = this->midiInterface.getSynthEvent(&event);
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
  MotorSynth motorSynth;
  motorSynth.setup(motor_control_pin);

  println("Monophonic Synthesizer (LED toggles on new event).");
  println("Please, read the docs to get some ideas on how to send MIDI to the device.");
  println("Reset the board to go back to the menu");

  motorSynth.printTunning();

  while (true)
  {
    int res = this->midiInterface.getSynthEvent(&event);
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
  println("Monophonic Synthesizer tuner (LED toggles on new event).");
  println("Press the MIDI note that you want to tune.");
  println("Use a MIDI control (fader, potentiometer) to perform the tunning.");
  println("The tunning change is relative to the control change, if you want to tune down a note");
  println("turn all right the potentiometer before pushing the note an then go left.");
  println("Release the MIDI note to stop the tunning (other notes pressed before this will be discarded).");
  println("Please, read the docs to get some ideas on how to send MIDI to the device.");
  println("Reset the board to go back to the menu");

  SynthEvent event = SynthEvent();
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
    int res = this->midiInterface.getSynthEvent(&event);

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