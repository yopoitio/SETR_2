#include <unity.h>
#include "../src/CMDPROC/cmdproc.h"
#include "../src/SENSORS/sensors.h"

void setUp(void) {
	resetRxBuffer();
	resetTxBuffer();
}

void tearDown(void) {
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
	TEST_ASSERT_EQUAL(-1, cmdProcessor());
}

void test_invalid_command_P(void) {
	rxChar('#');
	rxChar('P');
	rxChar('0');
	rxChar('8');
	rxChar('0');
	rxChar('!');
	TEST_ASSERT_EQUAL(-1, cmdProcessor());
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
	TEST_ASSERT_EQUAL(-1, cmdProcessor());
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
	rxChar('0');
	rxChar('7');
	rxChar('6');
	rxChar('!');
	TEST_ASSERT_EQUAL(0, cmdProcessor());
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
    TEST_ASSERT_EQUAL(-1, cmdProcessor());
}

void test_checksum_calculation(void) {
	unsigned char testBuf[] = {'A', '1', '2'};
	int expectedChecksum = ('A' + '1' + '2') % 256;
	TEST_ASSERT_EQUAL(expectedChecksum, calcChecksum(testBuf, 3));
}

void test_tx_buffer_output(void) {
	unsigned char buffer[10] = {0};
	int len;
	txChar('H');
	txChar('e');
	txChar('l');
	txChar('l');
	txChar('o');
	getTxBuffer(buffer, &len);
	TEST_ASSERT_EQUAL(0, strcmp((char*)buffer,"Hello"));
	TEST_ASSERT_EQUAL(5, len);
}

void test_call_cmd_in_the_middle_of_command(void) {
	rxChar('#');
	rxChar('R');
	rxChar('0');
	TEST_ASSERT_EQUAL(-1, cmdProcessor());
	rxChar('8');
	rxChar('2');
	rxChar('!');
	TEST_ASSERT_EQUAL(0, cmdProcessor());

	rxChar('#');
	rxChar('R');
	rxChar('0');
	rxChar('!');
	TEST_ASSERT_EQUAL(-1, cmdProcessor());
	rxChar('#');
	rxChar('R');
	rxChar('0');
	rxChar('8');
	rxChar('2');
	rxChar('!');
	TEST_ASSERT_EQUAL(0, cmdProcessor());

	rxChar('#');
	rxChar('R');
	rxChar('0');
	TEST_ASSERT_EQUAL(-1, cmdProcessor());
	rxChar('#');
	rxChar('R');
	rxChar('0');
	TEST_ASSERT_EQUAL(-1, cmdProcessor());
	rxChar('#');
	rxChar('R');
	rxChar('0');
	rxChar('8');
	rxChar('2');
	rxChar('!');
	TEST_ASSERT_EQUAL(0, cmdProcessor());

	rxChar('#');
	rxChar('R');
	rxChar('0');
	TEST_ASSERT_EQUAL(-1, cmdProcessor());
	rxChar('#');
	rxChar('R');
	rxChar('0');
	rxChar('!');
	rxChar('!');
	rxChar('!');
	TEST_ASSERT_EQUAL(-1, cmdProcessor());
	rxChar('#');
	rxChar('R');
	rxChar('0');
	rxChar('8');
	rxChar('2');
	rxChar('!');
	TEST_ASSERT_EQUAL(0, cmdProcessor());

	rxChar('#');
	rxChar('R');
	rxChar('0');
	rxChar('8');
	rxChar('2');
	TEST_ASSERT_EQUAL(-1, cmdProcessor());
	rxChar('!');
	rxChar('#');
	rxChar('R');
	rxChar('0');
	rxChar('8');
	rxChar('2');
	rxChar('!');
	TEST_ASSERT_EQUAL(0, cmdProcessor());
	TEST_ASSERT_EQUAL(0, cmdProcessor());
	TEST_ASSERT_EQUAL(-1, cmdProcessor());
}

void test_temp_sensor(void) {
	int returns;
	int history[20] = {__INT_MAX__};
	int history_confirm[20] = {-46, 34, -50, 2, -19, 21, -42, 7, -14, 40, -3, 58, 11, -27, 45, 0, 33, -22, 49, -35};

	returns = simulate_temp_sensor('A',NULL);
	TEST_ASSERT_EQUAL(-10, returns);
	returns = simulate_temp_sensor('P',NULL);
	TEST_ASSERT_EQUAL(23, returns);

	returns = simulate_temp_sensor('R',NULL);
	for(int i = 0; i < 25;i++) {
		simulate_temp_sensor('P',NULL);
	}

	simulate_temp_sensor('L',history);
	for(int i=0;i<20;i++) {
		TEST_ASSERT_EQUAL(history_confirm[i], history[i]);
	}
}

void test_hum_sensor(void) {
	int returns;
	int history[20] = {__INT_MAX__};
	int history_confirm[20] = {15, 0, 68, 77, 36, 53, 9, 22, 60, 45, 100, 31, 82, 99, 3, 48, 73, 11, 67, 25};

	returns = simulate_hum_sensor('A',NULL);
	TEST_ASSERT_EQUAL(34, returns);
	returns = simulate_hum_sensor('P',NULL);
	TEST_ASSERT_EQUAL(89, returns);

	returns = simulate_hum_sensor('R',NULL);
	for(int i = 0; i < 25;i++) {
		simulate_hum_sensor('P',NULL);
	}

	simulate_hum_sensor('L',history);
	for(int i=0;i<20;i++) {
		TEST_ASSERT_EQUAL(history_confirm[i], history[i]);
	}
}

void test_co2_sensor(void) {
	int returns;
	int history[20] = {__INT_MAX__};
	int history_confirm[20] = {13579, 5678, 15234, 400, 987, 16000, 1987, 13456, 765, 17000, 5432, 18765, 12004, 9876, 14235, 6789, 14987, 4321, 8765, 2345};

	returns = simulate_co2_sensor('A',NULL);
	TEST_ASSERT_EQUAL(18457, returns);
	returns = simulate_co2_sensor('P',NULL);
	TEST_ASSERT_EQUAL(1203, returns);

	returns = simulate_co2_sensor('R',NULL);
	for(int i = 0; i < 25;i++) {
		simulate_co2_sensor('P',NULL);
	}

	simulate_co2_sensor('L',history);
	for(int i=0;i<20;i++) {
		TEST_ASSERT_EQUAL(history_confirm[i], history[i]);
	}
}

void test_warning_during_return1(void) {
	unsigned char buffer[UART_TX_SIZE - 10] = {0};
	unsigned char bufferExpected[UART_TX_SIZE] = {0};
	int len = 0;
	
	strcpy((char *)buffer, "empty string or incomplete commmand");
	snprintf((char *)bufferExpected, sizeof(bufferExpected), "#W%s%03d!", buffer, calcChecksum(buffer, strlen((const char*)buffer)));

	memset(buffer, 0, sizeof(buffer));
	sendWarningErrorResponse(-1);
	getTxBuffer(buffer, &len);

	TEST_ASSERT_EQUAL_STRING(bufferExpected, buffer);
}

void test_error_during_return2(void) {
	unsigned char buffer[UART_TX_SIZE - 10] = {0};
	unsigned char bufferExpected[UART_TX_SIZE] = {0};
	int len = 0;
	
	strcpy((char *)buffer, "invalid command was found");
	snprintf((char *)bufferExpected, sizeof(bufferExpected), "#E%s%03d!", buffer, calcChecksum(buffer, strlen((const char*)buffer)));

	memset(buffer, 0, sizeof(buffer));
	sendWarningErrorResponse(-2);
	getTxBuffer(buffer, &len);

	TEST_ASSERT_EQUAL_STRING(bufferExpected, buffer);
}

void test_error_during_return3(void) {
	unsigned char buffer[UART_TX_SIZE - 10] = {0};
	unsigned char bufferExpected[UART_TX_SIZE] = {0};
	int len = 0;
	
	strcpy((char *)buffer, "checksum error was detected");
	snprintf((char *)bufferExpected, sizeof(bufferExpected), "#E%s%03d!", buffer, calcChecksum(buffer, strlen((const char*)buffer)));

	memset((char *)buffer, 0, sizeof(buffer));
	sendWarningErrorResponse(-3);
	getTxBuffer(buffer, &len);

	TEST_ASSERT_EQUAL_STRING(bufferExpected, buffer);
}

void test_error_during_return4(void) {
	unsigned char buffer[UART_TX_SIZE - 10] = {0};
	unsigned char bufferExpected[UART_TX_SIZE] = {0};
	int len = 0;
	
	strcpy((char *)buffer, "string format is wrong");
	snprintf((char *)bufferExpected, sizeof(bufferExpected), "#E%s%03d!", buffer, calcChecksum(buffer, strlen((const char*)buffer)));

	memset(buffer, 0, sizeof(buffer));
	sendWarningErrorResponse(-4);
	getTxBuffer(buffer, &len);

	TEST_ASSERT_EQUAL_STRING(bufferExpected, buffer);
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
	RUN_TEST(test_valid_command_R);
	RUN_TEST(test_invalid_command_R);
	RUN_TEST(test_checksum_calculation);
	RUN_TEST(test_tx_buffer_output);
	RUN_TEST(test_call_cmd_in_the_middle_of_command);
	RUN_TEST(test_temp_sensor);
	RUN_TEST(test_hum_sensor);
	RUN_TEST(test_co2_sensor);
	RUN_TEST(test_warning_during_return1);
	RUN_TEST(test_error_during_return2);
	RUN_TEST(test_error_during_return3);
	RUN_TEST(test_error_during_return4);
	
	return UNITY_END();
}