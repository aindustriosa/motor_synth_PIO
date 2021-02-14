
#include "cli.h"

void CLI::setup()
{
  Serial.begin(112500);
  print("Setup: blink");
  blink.setup();
  print("Setup: motor on pin", MOTOR_CONTROL_PIN);
  motor.setup(MOTOR_CONTROL_PIN);
  print("Setup: end");
}

void CLI::loop()
{
  print("Motor Synth: Menu");
  print("1: Change motor speed using this serial port");
  print("2: Sweep motor speed");
  waitBlinking();

  int value = getSerialPositiveValue();
  if (value == 1)
    loop_serial();
  if (value == 2)
    sweepMotor();

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

void CLI::loop_serial()
{
  print("Changing motor speed using this serial port:\nEnter a number to change the speed");
  print("Values from 0 to ", motor.getMaxSpeed());
  print("Enter three consecutive 0s to exit");
  waitBlinking();

  int zeros_to_exit = 3;

  while (zeros_to_exit > 0)
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

    if (value == 0)
    {
      zeros_to_exit = zeros_to_exit - 1;
    }
  }
}

void CLI::sweepMotor()
{
  print("Sweeping motor speed:");
  print("Please enter a value for the wait millisecs between speed changes (default is 100)");
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

  sweepSpeedUpwards(delay_between_changes_millis);
  sweepSpeedDownwards(delay_between_changes_millis);
  motor.setSpeed(0);
}

void CLI::sweepSpeedUpwards(int delay_between_changes_millis)
{
  int maxSpeed = motor.getMaxSpeed();
  for (int value = 0; value <= maxSpeed; value++)
  {
    motor.setSpeed(value);
    print("Speed:", value);
    blink.toggle();
    delay(delay_between_changes_millis);
  }
}

void CLI::sweepSpeedDownwards(int delay_between_changes_millis)
{
  int maxSpeed = motor.getMaxSpeed();
  for (int value = maxSpeed; value >= 0; value--)
  {
    motor.setSpeed(value);
    print("Speed:", value);
    blink.toggle();
    delay(delay_between_changes_millis);
  }
}