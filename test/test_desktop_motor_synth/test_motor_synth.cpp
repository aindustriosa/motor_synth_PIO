#include <ArduinoFake.h>

using namespace fakeit;

#include <unity.h>

#include "motor_synth.h"
#include "mock_serial_io.h"
#include "mock_motor_controller.h"
#include "synth_event/synth_event.h"

#define MAX_NUM_MOTORS 4
int control_pins[MAX_NUM_MOTORS] = {1, 2, 3, 4};
motor_synth::MockMotorController *motors[MAX_NUM_MOTORS];

motor_synth::MockSerialIO serial = motor_synth::MockSerialIO();

void setup_motors(int number_of_motors)
{
    if (number_of_motors > MAX_NUM_MOTORS)
    {
        throw "Trying to setup more than MAX_NUM_MOTORS!";
    }

    for (int i = 0; i < number_of_motors; i++)
    {
        motor_synth::MockMotorController *motor = new motor_synth::MockMotorController();
        motors[i] = motor;
        motors[i]->setup(control_pins[i]);
        motors[i]->setSpeed_lastSpeed = 0;
        motors[i]->setSpeed_timesCalled = 0;
    }
}

void teardown_motors(int number_of_motors)
{
    if (number_of_motors > MAX_NUM_MOTORS)
    {
        throw "Trying to delete more than MAX_NUM_MOTORS!";
    }

    for (int i = 0; i < number_of_motors; i++)
    {
        delete motors[i];
    }
}

void set_note_on(int note, motor_synth::SynthEvent *event)
{
    event->setChannel(1);
    event->setType(motor_synth::SynthEventType::NoteOn);
    event->setNote(note);
    event->setNoteVelocity(80);
}

void set_note_off(int note, motor_synth::SynthEvent *event)
{
    event->setChannel(1);
    event->setType(motor_synth::SynthEventType::NoteOff);
    event->setNote(note);
    event->setNoteVelocity(80);
}

void printEventPerMotor(motor_synth::MotorSynth *motorSynth,
                        motor_synth::MockSerialIO *serial)
{
    serial->println("--- Current events per motor ---");
    for (int motor_index = 0; motor_index < motorSynth->motors_len; motor_index++)
    {
        serial->print("- ");
        serial->print(motor_index, DEC);
        serial->print(" - ");
        motorSynth->getEventPerMotor()[motor_index].print(serial);
    }
    serial->println("--- --------- ---");
}

/**
 * GIVEN: A MotorSynth with one motor configured.
 * 
 * WHEN: Nothing is processed
 * 
 * THEN: Stack is empty
 */
void test_01_MotorSynth_setup_does_not_alter_stack(void)
{
    // Init
    const int number_of_motors = 1;
    setup_motors(number_of_motors);

    motor_synth::MotorSynth motorSynth;
    motorSynth.setup((motor_synth::MotorController **)motors, number_of_motors, &serial);

    // When

    // Then
    motorSynth.printStack();
    printEventPerMotor(&motorSynth, &serial);
    TEST_ASSERT_EQUAL(-1, motorSynth.get_eventsStackIndex());
    TEST_ASSERT_EQUAL(
        motor_synth::SynthEventType::InvalidType,
        motorSynth.getEventPerMotor()[0].getType());

    // Teardown
    teardown_motors(number_of_motors);
}

/**
 * GIVEN: A MotorSynth with one motor configured.
 * 
 * WHEN: A series of NoteOn are processed.
 * 
 * THEN: Each note increases the stack until the top is reached, then old notes are removed.
 */
void test_02_notes_on_update_stack(void)
{
    // Init
    const int number_of_motors = 1;
    const int stack_len = 4;
    int unique_notes[6] = {1, 2, 3, 4, 5, 6};
    motor_synth::SynthEvent event = motor_synth::SynthEvent();

    setup_motors(number_of_motors);

    motor_synth::MotorSynth motorSynth;
    motorSynth.setup(
        (motor_synth::MotorController **)motors,
        number_of_motors,
        stack_len,
        &serial);

    // When (no note)

    // Then
    TEST_ASSERT_EQUAL_INT(-1, motorSynth.get_eventsStackIndex());

    // (fill the stack)
    for (int stack_index = 0; stack_index < stack_len; stack_index++)
    {
        // When
        set_note_on(unique_notes[stack_index], &event);
        motorSynth.processEvent(&event);

        // Then
        motorSynth.printStack();
        printEventPerMotor(&motorSynth, &serial);

        TEST_ASSERT_EQUAL_INT(stack_index, motorSynth.get_eventsStackIndex());
        TEST_ASSERT_EQUAL(
            unique_notes[stack_index],
            motorSynth.get_eventsStack()[stack_index].getNote());
        TEST_ASSERT_EQUAL(
            motor_synth::SynthEventType::NoteOn,
            motorSynth.get_eventsStack()[stack_index].getType());
        TEST_ASSERT_EQUAL(
            motor_synth::SynthEventType::NoteOn,
            motorSynth.getEventPerMotor()[0].getType());
        TEST_ASSERT_EQUAL(
            unique_notes[stack_index],
            motorSynth.getEventPerMotor()[0].getNote());
    }

    // When (add new note to a filled stack)
    set_note_on(unique_notes[stack_len], &event);
    motorSynth.processEvent(&event);

    // Then (stack is shifted)
    motorSynth.printStack();
    printEventPerMotor(&motorSynth, &serial);

    TEST_ASSERT_EQUAL_INT(stack_len - 1, motorSynth.get_eventsStackIndex());
    for (int stack_index = 0; stack_index < stack_len; stack_index++)
    {
        TEST_ASSERT_EQUAL(
            motorSynth.get_eventsStack()[stack_index].getNote(),
            unique_notes[stack_index + 1]);
        TEST_ASSERT_EQUAL(
            motorSynth.get_eventsStack()[stack_index].getType(),
            motor_synth::SynthEventType::NoteOn);
    }
    for (int motor_number = 0; motor_number < number_of_motors; motor_number++)
    {
        TEST_ASSERT_EQUAL(
            motor_synth::SynthEventType::NoteOn,
            motorSynth.getEventPerMotor()[motor_number].getType());
        TEST_ASSERT_EQUAL(
            unique_notes[stack_len],
            motorSynth.getEventPerMotor()[motor_number].getNote());
    }

    // Teardown
    teardown_motors(number_of_motors);
}

/**
 * GIVEN: A MotorSynth with one motor configured. Some notes in the stack.
 * 
 * WHEN: A note is repeated.
 * 
 * THEN: The note is moved to the top, not duplicated.
 */
void test_03_notes_on_repeat_note(void)
{
    // Init
    const int number_of_motors = 1;
    const int stack_len = 4;
    int unique_notes[6] = {1, 2, 3, 4, 5, 6};
    motor_synth::SynthEvent event = motor_synth::SynthEvent();

    setup_motors(number_of_motors);

    motor_synth::MotorSynth motorSynth;
    motorSynth.setup(
        (motor_synth::MotorController **)motors, number_of_motors, stack_len, &serial);

    for (int i = 0; i < 3; i++)
    {
        set_note_on(unique_notes[i], &event);
        motorSynth.processEvent(&event);
    }

    // When (add a note that already exists in the stack)
    set_note_on(unique_notes[1], &event);
    motorSynth.processEvent(&event);

    // Then (note is moved to the top)
    motorSynth.printStack();
    printEventPerMotor(&motorSynth, &serial);

    int expected_stack_len = 3;
    int expected_notes_order[3] = {1, 3, 2};

    TEST_ASSERT_EQUAL_INT(expected_stack_len - 1, motorSynth.get_eventsStackIndex());
    for (int i = 0; i < 3; i++)
    {
        TEST_ASSERT_EQUAL(
            expected_notes_order[i],
            motorSynth.get_eventsStack()[i].getNote());
    }

    // Teardown
    teardown_motors(number_of_motors);
}

/**
 * GIVEN: A MotorSynth with 4 motors configured.
 * 
 * WHEN: A series of NoteOn are processed.
 * 
 * THEN: Motor speeds are updated accordingly.
 */
void test_04_notes_on_update_stack(void)
{
    // Init
    const int number_of_motors = 4;
    const int stack_len = 6;
    int unique_notes[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    motor_synth::SynthEvent event = motor_synth::SynthEvent();

    serial.println("setup motors:");
    setup_motors(number_of_motors);

    motor_synth::MotorSynth motorSynth;
    motorSynth.setup(
        (motor_synth::MotorController **)motors,
        number_of_motors,
        stack_len,
        &serial);

    // When
    int note_index = 0;
    set_note_on(unique_notes[note_index], &event);
    motorSynth.processEvent(&event);

    // Then (unison implementation: all motors with same speed)
    motorSynth.printStack();
    printEventPerMotor(&motorSynth, &serial);

    for (int motor_index = 0; motor_index < number_of_motors; motor_index++)
    {
        TEST_ASSERT_EQUAL_INT(1, motors[motor_index]->setSpeed_timesCalled);
        TEST_ASSERT_EQUAL_INT(
            motorSynth.getNoteVelocity(unique_notes[note_index]),
            motors[motor_index]->setSpeed_lastSpeed);
    }

    // Teardown
    teardown_motors(number_of_motors);
}

/**
 * GIVEN: A MotorSynth with 4 motors configured. A series of NoteOn are processed.
 * 
 * WHEN: A series of NoteOff are processed.
 * 
 * THEN: Motor speeds are updated accordingly.
 */
void test_05_notes_off_update_stack(void)
{
    // Init
    const int number_of_motors = 4;
    const int stack_len = 6;
    int unique_notes[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    motor_synth::SynthEvent event = motor_synth::SynthEvent();

    serial.println("setup motors:");
    setup_motors(number_of_motors);

    motor_synth::MotorSynth motorSynth;
    motorSynth.setup(
        (motor_synth::MotorController **)motors,
        number_of_motors,
        stack_len,
        &serial);

    int note_indexes[3] = {0, 1, 2};
    for (int note_index = 0; note_index < 3; note_index++) {
        set_note_on(unique_notes[note_index], &event);
        motorSynth.processEvent(&event);
    }
    motorSynth.printStack();
    printEventPerMotor(&motorSynth, &serial);

    // When
    set_note_off(unique_notes[2], &event);
    motorSynth.processEvent(&event);

    // Then (unison implementation: all motors with same speed)
    motorSynth.printStack();
    printEventPerMotor(&motorSynth, &serial);

    for (int motor_index = 0; motor_index < number_of_motors; motor_index++)
    {
        TEST_ASSERT_EQUAL_INT(
            motorSynth.getNoteVelocity(unique_notes[1]),
            motors[motor_index]->setSpeed_lastSpeed);
    }

    // When
    set_note_off(unique_notes[1], &event);
    motorSynth.processEvent(&event);

    // Then (unison implementation: all motors with same speed)
    motorSynth.printStack();
    printEventPerMotor(&motorSynth, &serial);

    for (int motor_index = 0; motor_index < number_of_motors; motor_index++)
    {
        TEST_ASSERT_EQUAL_INT(
            motorSynth.getNoteVelocity(unique_notes[0]),
            motors[motor_index]->setSpeed_lastSpeed);
    }

    // When
    set_note_off(unique_notes[0], &event);
    motorSynth.processEvent(&event);

    // Then (unison implementation: all motors with same speed)
    motorSynth.printStack();
    printEventPerMotor(&motorSynth, &serial);

    for (int motor_index = 0; motor_index < number_of_motors; motor_index++)
    {
        TEST_ASSERT_EQUAL_INT(
            DRONE_VELOCITY,
            motors[motor_index]->setSpeed_lastSpeed);
    }

    // Teardown
    teardown_motors(number_of_motors);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_01_MotorSynth_setup_does_not_alter_stack);
    RUN_TEST(test_02_notes_on_update_stack);
    RUN_TEST(test_03_notes_on_repeat_note);
    RUN_TEST(test_04_notes_on_update_stack);
    RUN_TEST(test_05_notes_off_update_stack);
    UNITY_END();

    return 0;
}
