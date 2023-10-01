#include "unity.h"
#include "BLAKE2s.h"
#include "Arduino.h"
#include "Controller/InboundMessage.h"
#include "Controller/OutboundMessage.h"


void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_inbound_message_invalid_data(void) {
    const unsigned char input[] = {0x88, 0x07, 0x8e, 0x00, 0x15};
    ACC::Controller::RemoteCommand::InboundMessage message(input, 5);
    TEST_ASSERT_EQUAL(0, message.getExtendedBytesLength());
    TEST_ASSERT_EQUAL(0, message.getFromAddress());
    TEST_ASSERT_EQUAL(0, message.getToAddress());
    TEST_ASSERT_EQUAL(0, message.getCommand());
    TEST_ASSERT_FALSE(message.isValid(0));
}

void test_inbound_message_without_data(void) {
    const unsigned char input[] = {
        ';', 0x8b, 0x00, 0x8a, 0x04, 0x88, 0x01, 0x89, 0x07, 'J', 'B', 0x89, 0x04, 0x8d, 0x0e, '\n', '/',
        0x12, 0x8b, 0x0e, 'k', 'y', 'I', 'k', 'd', 't', 0x00, 0x8a, 0x01, 0x8a, 0x02, 0x02
    };
    ACC::Controller::RemoteCommand::InboundMessage message(input, 32);
    TEST_ASSERT_EQUAL(0, message.getExtendedBytesLength());
    TEST_ASSERT_EQUAL(0xA1, message.getFromAddress());
    TEST_ASSERT_EQUAL(0xA2, message.getToAddress());
    TEST_ASSERT_EQUAL(0x02, message.getCommand());
    TEST_ASSERT_TRUE(message.isValid(0x74646A));
    TEST_ASSERT_FALSE(message.isValid(0x74646C));
}

void test_inbound_message_without_data_invalid_hmac(void) {
    const unsigned char input[] = {
        ';', 0x8b, 0x00, 0x8a, 0x04, 0x88, 0x02, 0x89, 0x07, 'J', 'B', 0x89, 0x04, 0x8d, 0x0e, '\n', '/',
        0x12, 0x8b, 0x0e, 'k', 'y', 'I', 'k', 'd', 't', 0x00, 0x8a, 0x01, 0x8a, 0x02, 0x02
    };
    ACC::Controller::RemoteCommand::InboundMessage message(input, 32);
    TEST_ASSERT_EQUAL(0, message.getExtendedBytesLength());
    TEST_ASSERT_EQUAL(0xA1, message.getFromAddress());
    TEST_ASSERT_EQUAL(0xA2, message.getToAddress());
    TEST_ASSERT_EQUAL(0x02, message.getCommand());
    TEST_ASSERT_FALSE(message.isValid(0));
}

void test_inbound_message_with_data(void) {
    const unsigned char input[] = {
        0x88, 0x06, 0x8f, 0x0c, 0x8b, 0x0f, 0x8f, '\r', 0x7f, 'o', '\\', 0x89, 0x04, 'g', 'P', 0x0e, '`', 0x8c, '\r',
        'M', 0x88, 0x05, '8', 'k', 'd', 't', 0x00, 0x01, '}', '~', 0x8f, 0x0f, 0x7f, 0x8c, 0x00, 0x88, 0x00
    };
    ACC::Controller::RemoteCommand::InboundMessage message(input, 37);
    TEST_ASSERT_EQUAL(4, message.getExtendedBytesLength());
    TEST_ASSERT_EQUAL(0x01, message.getFromAddress());
    TEST_ASSERT_EQUAL(0x7D, message.getToAddress());
    TEST_ASSERT_EQUAL(0x7E, message.getCommand());
    TEST_ASSERT_TRUE(message.isValid(0x74646A));
    TEST_ASSERT_FALSE(message.isValid(0x74646C));

    unsigned long value;
    memcpy(&value, message.getExtendedBytes(), 4);
    TEST_ASSERT_EQUAL(0x80C07FFF, value);
}

void test_inbound_message_with_data_invalid_hmac(void) {
    const unsigned char input[] = {
        0x88, 0x06, 0x8f, 0x0c, 0x8b, 0x0f, 0x8f, '\r', 0x7e, 'o', '\\', 0x89, 0x04, 'g', 'P', 0x0e, '`', 0x8c, '\r',
        'M', 0x88, 0x05, '8', 'k', 'd', 't', 0x00, 0x01, '}', '~', 0x8f, 0x0f, 0x7f, 0x8c, 0x00, 0x88, 0x00
    };
    ACC::Controller::RemoteCommand::InboundMessage message(input, 37);
    TEST_ASSERT_EQUAL(4, message.getExtendedBytesLength());
    TEST_ASSERT_EQUAL(0x01, message.getFromAddress());
    TEST_ASSERT_EQUAL(0x7D, message.getToAddress());
    TEST_ASSERT_EQUAL(0x7E, message.getCommand());
    TEST_ASSERT_FALSE(message.isValid(0));

    unsigned long value;
    memcpy(&value, message.getExtendedBytes(), 4);
    TEST_ASSERT_EQUAL(0x80C07FFF, value);
}

void test_outbound_message_without_data(void) {
    ACC::Controller::RemoteCommand::OutboundMessage message(0xA1, 0xA2, 0x02, 0x74646B);
    const unsigned char expectedData[] = {
        0xff, ' ', ';', 0x8b, 0x00, 0x8a, 0x04, 0x88, 0x01, 0x89, 0x07, 'J', 'B', 0x89, 0x04, 0x8d, 0x0e, '\n', '/',
        0x12, 0x8b, 0x0e, 'k', 'y', 'I', 'k', 'd', 't', 0x00, 0x8a, 0x01, 0x8a, 0x02, 0x02
    };
    TEST_ASSERT_EQUAL(34, message.getLength());
    TEST_ASSERT_EQUAL_MEMORY(expectedData, message.getEncodedBytes(), 34);
    TEST_ASSERT_EQUAL(0xC0, message.getEncodedBytes()[0] & 0xC0);
    for (unsigned int i = 1; i < 30; i++) {
        TEST_ASSERT_NOT_EQUAL(0xC0, message.getEncodedBytes()[i] & 0xC0);
    }
}

void test_outbound_message_with_data(void) {
    unsigned short data[2] = {0x7FFF, 0x80C0};
    ACC::Controller::RemoteCommand::OutboundMessage message(0x01, 0x7D, 0x7E, 0x74646B, data, 2 * sizeof(unsigned short));
    const unsigned char expectedData[] = {
        0xff, '%', 0x88, 0x06, 0x8f, 0x0c, 0x8b, 0x0f, 0x8f, '\r', 0x7f, 'o', '\\', 0x89, 0x04, 'g', 'P', 0x0e, '`', 0x8c, '\r',
        'M', 0x88, 0x05, '8', 'k', 'd', 't', 0x00, 0x01, '}', '~', 0x8f, 0x0f, 0x7f, 0x8c, 0x00, 0x88, 0x00
    };
    TEST_ASSERT_EQUAL(39, message.getLength());
    TEST_ASSERT_EQUAL_MEMORY(expectedData, message.getEncodedBytes(), 39);
    TEST_ASSERT_EQUAL(0xC0, message.getEncodedBytes()[0] & 0xC0);
    for (unsigned int i = 1; i < 42; i++) {
        TEST_ASSERT_NOT_EQUAL(0xC0, message.getEncodedBytes()[i] & 0xC0);
    }
}

int runUnityTests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_inbound_message_invalid_data);
    RUN_TEST(test_inbound_message_without_data);
    RUN_TEST(test_inbound_message_without_data_invalid_hmac);
    RUN_TEST(test_inbound_message_with_data);
    RUN_TEST(test_inbound_message_with_data_invalid_hmac);
    RUN_TEST(test_outbound_message_without_data);
    RUN_TEST(test_outbound_message_with_data);
    return UNITY_END();
}

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    runUnityTests();
}

void loop() {}