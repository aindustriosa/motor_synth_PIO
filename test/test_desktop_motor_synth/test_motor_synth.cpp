#include "fakeit/fakeit.hpp"
#include <unity.h>

using namespace fakeit;

#include "motor_synth.h"
#include "serial.h"

const int MOTOR_CONTROL_PIN = 9;


void setUp(void)
{
    //ArduinoFakeReset();
}

// Used only for FakeIt testing
void test_WHEN_MotorSynth_instantiated_THEN_nothing_happens(void) {
    Mock<motor_synth::MotorController> mock_motor_controller;
    motor_synth::MotorController& motor  = mock_motor_controller.get();

    Mock<motor_synth::SerialIO> mock_serial_io;
    motor_synth::SerialIO& serialIO  = mock_serial_io.get();

    MotorSynth motorSynth;
    motorSynth.setup(&motor, &serialIO);
    VerifyNoOtherInvocations(mock_motor_controller);
}

void test_WHEN__THEN_(void) {
    Mock<motor_synth::MotorController> mock_motor_controller;
 //   When(Method(mock_motor_controller, setup));
 //   When(Method(mock_motor_controller, setSpeed));
 //   When(Method(mock_motor_controller, getMaxSpeed)).Return(2000);
    motor_synth::MotorController& motor  = mock_motor_controller.get();

    Mock<motor_synth::SerialIO> mock_serial_io;
//    When(OverloadedMethod(mock_serial_io, println, size_t(const char *)));
    motor_synth::SerialIO& serialIO  = mock_serial_io.get();

    MotorSynth motorSynth;
    motorSynth.setup(&motor, &serialIO);

  //  Verify(OverloadedMethod(mock_serial_io, println, size_t(const char *))).Once();
    VerifyNoOtherInvocations(mock_motor_controller);
}


int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_WHEN_MotorSynth_instantiated_THEN_nothing_happens);
    RUN_TEST(test_WHEN__THEN_);
    UNITY_END();

    return 0;
}

