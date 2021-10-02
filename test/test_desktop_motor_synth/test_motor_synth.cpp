#include "fakeit/fakeit.hpp"  // https://github.com/eranpeer/FakeIt
#include <unity.h>

using namespace fakeit;

#include "motor_synth.h"
#include "mock_serial_io.h"


motor_synth::MotorController *motors;
motor_synth::MockSerialIO serial = motor_synth::MockSerialIO();


// Used only for FakeIt testing. We don't need a MotorController in these tests, 
// We could make an empty stub or fake it. The same for the serial printing.
void test_WHEN_MotorSynth_instantiated_THEN_nothing_happens(void) {
    // Instantiates a Mock object for the motor controller. 
    // Note that we use an interface/virtual class
    // to define the object's interfaces.
    Mock<motor_synth::MotorController> mock_motor_controller;
    // Get a motor instance.
    motors[0] = mock_motor_controller.get();

    motor_synth::MotorSynth motorSynth;
    motorSynth.setup(motors, 1, &serial);

    VerifyNoOtherInvocations(mock_motor_controller);
}

void test_WHEN_MotorSynth_instantiated_THEN_events_stack_is_empty(void) {
    Mock<motor_synth::MotorController> mock_motor_controller;
    motors[0] = mock_motor_controller.get();
    motor_synth::MotorSynth motorSynth;
    motorSynth.setup(motors, 1, &serial);

    motorSynth.printStack();

    TEST_ASSERT_EQUAL(-1, motorSynth.get_eventsStackIndex());
}

/**
 * Given a motorSynth, a different set of MIDI events arrive. The events stack is 
 * updated accordingly.
 **/
void test_WHEN_MotorSynth_instantiated_on_events_THEN_events_stack_changes_accordingly(void) {
    Mock<motor_synth::MotorController> mock_motor_controller;
    When(Method(mock_motor_controller, setSpeed)).AlwaysReturn();
    motors[0] = mock_motor_controller.get();
    motor_synth::MotorSynth motorSynth;
    motorSynth.setup(motors, 1, &serial);

    motorSynth.printStack();

    TEST_ASSERT_EQUAL_INT(-1, motorSynth.get_eventsStackIndex());

    SynthEvent event = SynthEvent();
    event.setChannel(1);
    event.setType(SynthEventType::NoteOn);
    event.setNote(64);
    event.setNoteVelocity(80);
/*
    serial.println("Hi from MotorSynth::processEvent");
    motorSynth.processEvent(&event);

    motorSynth.printStack();
    TEST_ASSERT_EQUAL_INT(-1, motorSynth.get_eventsStackIndex());

    TEST_ASSERT(motorSynth.get_eventsStackIndex() == 0);
    TEST_ASSERT_EQUAL(motorSynth.get_eventsStack()[0].getNote(), 1);
    TEST_ASSERT_EQUAL(motorSynth.get_eventsStack()[0].getType(), SynthEventType::NoteOn);
*/
}

int main(int argc, char **argv) {

    UNITY_BEGIN();
    RUN_TEST(test_WHEN_MotorSynth_instantiated_THEN_nothing_happens);
    RUN_TEST(test_WHEN_MotorSynth_instantiated_THEN_events_stack_is_empty);
    RUN_TEST(test_WHEN_MotorSynth_instantiated_on_events_THEN_events_stack_changes_accordingly);
    UNITY_END();

    return 0;
}

