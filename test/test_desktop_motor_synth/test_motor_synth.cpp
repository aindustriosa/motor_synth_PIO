#include <ArduinoFake.h>

using namespace fakeit;

#include <unity.h>

#include "motor_synth.h"
#include "mock_serial_io.h"
#include "mock_motor_controller.h"

#define MAX_NUM_MOTORS 4
int control_pins[MAX_NUM_MOTORS] = {1,2,3,4};
motor_synth::MockMotorController motors[MAX_NUM_MOTORS];

void setup_motors(int number_of_motors) {
    for (int i = 0; i < number_of_motors; i++) {
        motors[i].setup(control_pins[i]);
    }
}

motor_synth::MockSerialIO serial = motor_synth::MockSerialIO();

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
    setup_motors(1);

    motor_synth::MotorSynth motorSynth;
    motorSynth.setup(motors, 1, &serial);

    // When
    motorSynth.printStack();

    // Then
    TEST_ASSERT_EQUAL(-1, motorSynth.get_eventsStackIndex());
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
    int num_of_motors = 1;
    int stack_len = 4;
    int unique_notes[6] = {1, 2, 3, 4, 5, 6};

    setup_motors(num_of_motors);

    motor_synth::MotorSynth motorSynth;
    motorSynth.setup(motors, num_of_motors, stack_len, &serial);

    // When (no note)

    // Then
    TEST_ASSERT_EQUAL_INT(-1, motorSynth.get_eventsStackIndex());

    // (fill the stack)
    for (int i = 0; i < stack_len; i ++){
        // When
        motor_synth::SynthEvent event = motor_synth::SynthEvent();
        set_note_on(unique_notes[i], &event);
        motorSynth.processEvent(&event);

        // Then
        motorSynth.printStack();
        TEST_ASSERT_EQUAL_INT(i, motorSynth.get_eventsStackIndex());
        TEST_ASSERT_EQUAL(motorSynth.get_eventsStack()[i].getNote(), unique_notes[i]);
        TEST_ASSERT_EQUAL(motorSynth.get_eventsStack()[i].getType(), motor_synth::SynthEventType::NoteOn);
    }

    // When (add new note to a filled stack)
    motor_synth::SynthEvent event = motor_synth::SynthEvent();
    set_note_on(unique_notes[stack_len], &event);
    motorSynth.processEvent(&event);

    // Then (stack is sifted)
    motorSynth.printStack();
    TEST_ASSERT_EQUAL_INT(stack_len -1 , motorSynth.get_eventsStackIndex());
    for (int i = 0; i < stack_len; i ++){
        TEST_ASSERT_EQUAL(motorSynth.get_eventsStack()[i].getNote(), unique_notes[i+1]);
        TEST_ASSERT_EQUAL(motorSynth.get_eventsStack()[i].getType(), motor_synth::SynthEventType::NoteOn);
    }
}

int main(int argc, char **argv) {

    UNITY_BEGIN();
    RUN_TEST(test_01_MotorSynth_setup_does_not_alter_stack);
    RUN_TEST(test_02_notes_on_update_stack);
    UNITY_END();

    return 0;
}

