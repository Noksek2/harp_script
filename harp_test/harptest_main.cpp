#include <stdint.h>
extern void hashtable_test(uint32_t);
extern void harpsymbol_test();
extern void test_init();
extern void myjson_test();
int main() {
	test_init();
	//myjson_test();
	harpsymbol_test();
	//hashtable_test(256);
	

	return 0;
}