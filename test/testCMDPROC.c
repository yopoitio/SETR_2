#include <unity.h>
#include "../src/CMDPROC/cmdproc.h"

void setUp(void) {
	return;
}

void tearDown(void) {
	return;
}

void test_CMDPROC_Testing1(void) {
    TEST_ASSERT_EQUAL_INT16(196, calcChecksum("Pt196",2));
    TEST_ASSERT_EQUAL_INT16(0, calcChecksum("Pt197",2));
}

void test_CMDPROC_Testing2(void) {
    // meter mais testes
    // criar mais funções
}

int main(void)
{
	UNITY_BEGIN();
	
	RUN_TEST(test_CMDPROC_Testing1);			
	RUN_TEST(test_CMDPROC_Testing2);
		
	return UNITY_END();
}
