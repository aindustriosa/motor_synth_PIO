#include <ArduinoFake.h>
#include <unity.h>

using namespace fakeit;


void test_function_simple_test(void) {
    TEST_ASSERT_EQUAL(32, 30 + 2);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_function_simple_test);
    UNITY_END();

    return 0;
}
