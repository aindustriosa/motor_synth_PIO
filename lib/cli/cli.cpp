
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