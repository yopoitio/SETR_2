#include <unity.h>
#include "../src/CMDPROC/cmdproc.h"
#include "../src/SENSORS/sensors.h"

void setUp(void) {
	resetRxBuffer();
	resetTxBuffer();
	return NULL;
}

void tearDown(void) {
	return NULL;
}

void test_valid_command_A(void) {
	rxChar('#');
	rxChar('A');
	rxChar('0');
	rxChar('6');
	rxChar('5');
	rxChar('!');
	TEST_ASSERT_EQUAL(0, cmdProcessor());
}

void test_invalid_command_A(void) {
	rxChar('#');
	rxChar('A');
	rxChar('6');
	rxChar('5');
	rxChar('!');
	TEST_ASSERT_EQUAL(-4, cmdProcessor());
}

void test_invalid_command_P(void) {
	rxChar('#');
	rxChar('P');
	rxChar('0');
	rxChar('8');
	rxChar('0');
	rxChar('!');
	TEST_ASSERT_EQUAL(-2, cmdProcessor());
}

void test_invalid_checksum(void) {
	rxChar('#');
	rxChar('A');
	rxChar('0');
	rxChar('9');
	rxChar('9');
	rxChar('!');
	TEST_ASSERT_EQUAL(-3, cmdProcessor());
}

void test_unkown_command(void) {
	rxChar('#');
	rxChar('X');
	rxChar('0');
	rxChar('0');
	rxChar('0');
	rxChar('!');
	TEST_ASSERT_EQUAL(-2, cmdProcessor());
}

void test_missing_start_symbol(void) {
	rxChar('A');
	rxChar('0');
	rxChar('0');
	rxChar('0');
	rxChar('!');
	TEST_ASSERT_EQUAL(-4, cmdProcessor());
}

void test_missing_end_symbol(void) {
	rxChar('#');
	rxChar('A');
	rxChar('0');
	rxChar('0');
	rxChar('0');
	TEST_ASSERT_EQUAL(-1, cmdProcessor());
}

void test_valid_command_L(void) {
	rxChar('#');
	rxChar('L');
	rxChar('c'); // CO2 variable
	rxChar('1');
	rxChar('7');
	rxChar('5');
	rxChar('!');
	TEST_ASSERT_EQUAL(0, cmdProcessor());
}

void test_invalid_command_L(void) {
	rxChar('#');
    rxChar('L');
    rxChar('m');
    rxChar('1');
    rxChar('8');
	rxChar('5');
    rxChar('!');
    TEST_ASSERT_EQUAL(-2, cmdProcessor());
}

void test_valid_command_R(void) {
	rxChar('#');
	rxChar('R');
	rxChar('0');
	rxChar('8');
	rxChar('2');
	rxChar('!');
	TEST_ASSERT_EQUAL(0, cmdProcessor());
}

void test_invalid_command_R(void) {
	rxChar('#');
    rxChar('R');
    rxChar('0');
    rxChar('2');
    rxChar('!');
    TEST_ASSERT_EQUAL(-4, cmdProcessor());
}

void test_checksum_calculation(void) {
	unsigned char testBuf[] = {'A', '1', '2'};
	int expectedChecksum = ('A' + '1' + '2') % 256;
	TEST_ASSERT_EQUAL(expectedChecksum, calcChecksum(testBuf, 3));
}

void test_tx_buffer_output(void) {
	unsigned char buffer[10];
	int len;
	txChar('H');
	txChar('e');
	txChar('l');
	txChar('l');
	txChar('o');
	getTxBuffer(buffer, &len);
	TEST_ASSERT_NOT_EQUAL(0, len);
}

int main(void) {
	UNITY_BEGIN();
	
	RUN_TEST(test_valid_command_A);
	RUN_TEST(test_invalid_command_A);
	RUN_TEST(test_invalid_command_P);
	RUN_TEST(test_invalid_checksum);
	RUN_TEST(test_unkown_command);
	RUN_TEST(test_missing_start_symbol);
	RUN_TEST(test_missing_end_symbol);
	RUN_TEST(test_valid_command_L);
	RUN_TEST(test_invalid_command_L);
	RUN_TEST(test_valid_command_R);
	RUN_TEST(test_invalid_command_R);
	RUN_TEST(test_checksum_calculation);
	RUN_TEST(test_tx_buffer_output);
	
	return UNITY_END();
}