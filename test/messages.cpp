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
    TEST_ASSERT_EQUAL(0, message.getDataLength());
    TEST_ASSERT_EQUAL(0, message.getRecipient());
    TEST_ASSERT_EQUAL(0, message.getCommand());
    TEST_ASSERT_FALSE(message.isValid(0));
}

void test_inbound_message_without_data(void) {
    const unsigned char input[] = {
        0x88, 0x08, 0x8e, 0x00, 0x15, ':', 0x8a, 0x06, '1', '?', 0x02, '4', 0x8f,
        0x04, 0x13, ';', '%', 'r', 0x8e, 0x07, 0x12, 'k', 'd', 't', 0x00, 0x8a, 0x02, 0x02
    };
    ACC::Controller::RemoteCommand::InboundMessage message(input, 28);
    TEST_ASSERT_EQUAL(0, message.getDataLength());
    TEST_ASSERT_EQUAL(0xA2, message.getRecipient());
    TEST_ASSERT_EQUAL(0x02, message.getCommand());
    TEST_ASSERT_TRUE(message.isValid(0x74646A));
    TEST_ASSERT_FALSE(message.isValid(0x74646C));
}

void test_inbound_message_without_data_invalid_hmac(void) {
    const unsigned char input[] = {
        0x88, 0x07, 0x8e, 0x00, 0x15, ':', 0x8a, 0x06, '1', '?', 0x02, '4', 0x8f,
        0x04, 0x13, ';', '%', 'r', 0x8e, 0x07, 0x12, 'k', 'd', 't', 0x00, 0x8a, 0x02, 0x02
    };
    ACC::Controller::RemoteCommand::InboundMessage message(input, 28);
    TEST_ASSERT_EQUAL(0, message.getDataLength());
    TEST_ASSERT_EQUAL(0xA2, message.getRecipient());
    TEST_ASSERT_EQUAL(0x02, message.getCommand());
    TEST_ASSERT_FALSE(message.isValid(0));
}

void test_inbound_message_with_data(void) {
    const unsigned char input[] = {
        0x89, 0x00, 0x8b, 0x04, 0x8d, 0x0e, 0x8c, 0x08, '3', 0x89, 0x08, 0x8c,
        0x0e, 0x8c, 0x03, '5', 0x1f, 0x89, 0x06, 0x02, 0x89, 0x0f, 0x8e, 0x01, 0x88, 0x07,
        'l', 'k', 'd', 't', 0x00, '}', '~', 0x8f, 0x0f, 0x7f, 0x8c, 0x00, 0x88, 0x00
    };
    ACC::Controller::RemoteCommand::InboundMessage message(input, 40);
    TEST_ASSERT_EQUAL(4, message.getDataLength());
    TEST_ASSERT_EQUAL(0x7D, message.getRecipient());
    TEST_ASSERT_EQUAL(0x7E, message.getCommand());
    TEST_ASSERT_TRUE(message.isValid(0x74646A));
    TEST_ASSERT_FALSE(message.isValid(0x74646C));

    unsigned long value;
    memcpy(&value, message.getData(), 4);
    TEST_ASSERT_EQUAL(0x80C07FFF, value);
}

void test_inbound_message_with_data_invalid_hmac(void) {
    const unsigned char input[] = {
        0x89, 0x00, 0x8b, 0x04, 0x8d, 0x0e, 0x8c, 0x08, '3', 0x89, 0x07, 0x8c,
        0x0e, 0x8c, 0x03, '5', 0x1f, 0x89, 0x06, 0x02, 0x89, 0x0f, 0x8e, 0x01, 0x88, 0x07,
        'l', 'k', 'd', 't', 0x00, '}', '~', 0x8f, 0x0f, 0x7f, 0x8c, 0x00, 0x88, 0x00
    };
    ACC::Controller::RemoteCommand::InboundMessage message(input, 40);
    TEST_ASSERT_EQUAL(4, message.getDataLength());
    TEST_ASSERT_EQUAL(0x7D, message.getRecipient());
    TEST_ASSERT_EQUAL(0x7E, message.getCommand());
    TEST_ASSERT_FALSE(message.isValid(0));

    unsigned long value;
    memcpy(&value, message.getData(), 4);
    TEST_ASSERT_EQUAL(0x80C07FFF, value);
}

void test_outbound_message_without_data(void) {
    ACC::Controller::RemoteCommand::OutboundMessage message(0xA2, 0x02, 0x74646B);
    const unsigned char expectedData[] = {
        0xff, 0x1c, 0x88, 0x08, 0x8e, 0x00, 0x15, ':', 0x8a, 0x06, '1', '?', 0x02, '4', 0x8f,
        0x04, 0x13, ';', '%', 'r', 0x8e, 0x07, 0x12, 'k', 'd', 't', 0x00, 0x8a, 0x02, 0x02
    };
    TEST_ASSERT_EQUAL(30, message.getDataLength());
    TEST_ASSERT_EQUAL_MEMORY(expectedData, message.getEncodedData(), 30);
    TEST_ASSERT_EQUAL(0xC0, message.getEncodedData()[0] & 0xC0);
    for (unsigned int i = 1; i < 30; i++) {
        TEST_ASSERT_NOT_EQUAL(0xC0, message.getEncodedData()[i] & 0xC0);
    }
}

void test_outbound_message_with_data(void) {
    unsigned short data[2] = {0x7FFF, 0x80C0};
    ACC::Controller::RemoteCommand::OutboundMessage message(0x7D, 0x7E, 0x74646B, data, 2 * sizeof(unsigned short));
    const unsigned char expectedData[] = {
        0xff, '(', 0x89, 0x00, 0x8b, 0x04, 0x8d, 0x0e, 0x8c, 0x08, '3', 0x89, 0x08, 0x8c,
        0x0e, 0x8c, 0x03, '5', 0x1f, 0x89, 0x06, 0x02, 0x89, 0x0f, 0x8e, 0x01, 0x88, 0x07,
        'l', 'k', 'd', 't', 0x00, '}', '~', 0x8f, 0x0f, 0x7f, 0x8c, 0x00, 0x88, 0x00
    };
    TEST_ASSERT_EQUAL(42, message.getDataLength());
    TEST_ASSERT_EQUAL_MEMORY(expectedData, message.getEncodedData(), 42);
    TEST_ASSERT_EQUAL(0xC0, message.getEncodedData()[0] & 0xC0);
    for (unsigned int i = 1; i < 42; i++) {
        TEST_ASSERT_NOT_EQUAL(0xC0, message.getEncodedData()[i] & 0xC0);
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