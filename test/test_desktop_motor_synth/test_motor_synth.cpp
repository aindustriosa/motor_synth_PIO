#include <ArduinoFake.h>

using namespace fakeit;

#include <unity.h>

#include "motor_synth.h"
#include "mock_serial_io.h"
#include "mock_motor_controller.h"

#define MAX_NUM_MOTORS 4
int control_pins[MAX_NUM_MOTORS] = {1,2,3,4};
motor_synth::MockMotorController *motors[MAX_NUM_MOTORS];

motor_synth::MockSerialIO serial = motor_synth::MockSerialIO();

void setup_motors(int number_of_motors) {
    if (number_of_motors > MAX_NUM_MOTORS) {
        throw "Trying to setup more than MAX_NUM_MOTORS!";
    }

    for (int i = 0; i < number_of_motors; i++) {
        motor_synth::MockMotorController* motor = new motor_synth::MockMotorController();
        motors[i] = motor;
        motors[i]->setup(control_pins[i]);
        motors[i]->setSpeed_lastSpeed = 0;
        motors[i]->setSpeed_timesCalled = 0;
    }
}

void teardown_motors(int number_of_motors) {
    if (number_of_motors > MAX_NUM_MOTORS) {
        throw "Trying to delete more than MAX_NUM_MOTORS!";
    }

    for (int i = 0; i < number_of_motors; i++) {
        delete motors[i];
    }
}

void set_note_on(int note, motor_synth::SynthEvent * event) {
    event->setChannel(1);
    event->setType(motor_synth::SynthEventType::NoteOn);
    event->setNote(note);
    event->setNoteVelocity(80);
}


/**
 * GIVEN: A MotorSynth with one motor configured.
 * 
 * WHEN: Nothing is processed
 * 
 * THEN: Stack is empty
 */
void test_01_MotorSynth_setup_does_not_alter_stack(void) {
    // Init
    const int number_of_motors = 1;
    setup_motors(number_of_motors);

    motor_synth::MotorSynth motorSynth;
    motorSynth.setup((motor_synth::MotorController**)motors, number_of_motors, &serial);

    // When
    motorSynth.printStack();

    // Then
    TEST_ASSERT_EQUAL(-1, motorSynth.get_eventsStackIndex());

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
void test_02_notes_on_update_stack(void) {
    // Init
    const int number_of_motors = 1;
    const int stack_len = 4;
    int unique_notes[6] = {1, 2, 3, 4, 5, 6};
    motor_synth::SynthEvent event = motor_synth::SynthEvent();

    setup_motors(number_of_motors);

    motor_synth::MotorSynth motorSynth;
    motorSynth.setup((motor_synth::MotorController**)motors, number_of_motors, stack_len, &serial);

    // When (no note)

    // Then
    TEST_ASSERT_EQUAL_INT(-1, motorSynth.get_eventsStackIndex());

    // (fill the stack)
    for (int i = 0; i < stack_len; i ++){
        // When
        set_note_on(unique_notes[i], &event);
        motorSynth.processEvent(&event);

        // Then
        motorSynth.printStack();
        TEST_ASSERT_EQUAL_INT(i, motorSynth.get_eventsStackIndex());
        TEST_ASSERT_EQUAL(motorSynth.get_eventsStack()[i].getNote(), unique_notes[i]);
        TEST_ASSERT_EQUAL(motorSynth.get_eventsStack()[i].getType(), motor_synth::SynthEventType::NoteOn);
    }

    // When (add new note to a filled stack)
    set_note_on(unique_notes[stack_len], &event);
    motorSynth.processEvent(&event);

    // Then (stack is shifted)
    motorSynth.printStack();
    TEST_ASSERT_EQUAL_INT(stack_len -1 , motorSynth.get_eventsStackIndex());
    for (int i = 0; i < stack_len; i ++){
        TEST_ASSERT_EQUAL(motorSynth.get_eventsStack()[i].getNote(), unique_notes[i+1]);
        TEST_ASSERT_EQUAL(motorSynth.get_eventsStack()[i].getType(), motor_synth::SynthEventType::NoteOn);
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
void test_03_notes_on_repeat_note(void) {
    // Init
    const int number_of_motors = 1;
    const int stack_len = 4;
    int unique_notes[6] = {1, 2, 3, 4, 5, 6};
    motor_synth::SynthEvent event = motor_synth::SynthEvent();

    setup_motors(number_of_motors);

    motor_synth::MotorSynth motorSynth;
    motorSynth.setup((motor_synth::MotorController**)motors, number_of_motors, stack_len, &serial);

    for (int i = 0; i < 3; i++) {
        set_note_on(unique_notes[i], &event);
        motorSynth.processEvent(&event);
    }

    // When (add a note that already exists in the stack)
    set_note_on(unique_notes[1], &event);
    motorSynth.processEvent(&event);

    // Then (note is moved to the top)
    motorSynth.printStack();
    int expected_stack_len = 3;
    int expected_notes_order[3] = {1, 3, 2};
    
    TEST_ASSERT_EQUAL_INT(expected_stack_len -1 , motorSynth.get_eventsStackIndex());
    for (int i = 0; i < 3; i++){
        TEST_ASSERT_EQUAL(expected_notes_order[i],
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
void test_04_notes_on_update_stack(void) {
    // Init
    const int number_of_motors = 4;
    const int stack_len = 6;
    int unique_notes[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    motor_synth::SynthEvent event = motor_synth::SynthEvent();

    serial.println("setup motors:");
    setup_motors(number_of_motors);

    motor_synth::MotorSynth motorSynth;
    motorSynth.setup((motor_synth::MotorController**) motors, 
                     number_of_motors, 
                     stack_len, 
                     &serial);
    
    // When
    int note_index = 0;
    set_note_on(unique_notes[note_index], &event);
    motorSynth.processEvent(&event);

    // Then (unison implementation: all motors with same speed)
    motorSynth.printStack();
    for (int i = 0; i < number_of_motors; i++) {
        TEST_ASSERT_EQUAL_INT(1, motors[i]->setSpeed_timesCalled);
        TEST_ASSERT_EQUAL_INT(
            motorSynth.getNoteVelocity(unique_notes[note_index]),
            motors[i]->setSpeed_lastSpeed);
    }
    
    // Teardown
    teardown_motors(number_of_motors);
}

int main(int argc, char **argv) {

    UNITY_BEGIN();
    RUN_TEST(test_01_MotorSynth_setup_does_not_alter_stack);
    RUN_TEST(test_02_notes_on_update_stack);
    RUN_TEST(test_03_notes_on_repeat_note);
    RUN_TEST(test_04_notes_on_update_stack);
    UNITY_END();

    return 0;
}

