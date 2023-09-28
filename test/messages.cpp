#include "unity.h"
#include "Arduino.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_blake2(void) {

}

int runUnityTests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_blake2);
    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}