#include <stdint.h>
//extern void hashtable_test(uint32_t);
//extern void harpsymbol_test();
extern void test_init();
extern void myjson_test();

#include <h_data.h>
int main() {
	test_init();
	//myjson_test();
	//harpsymbol_test();
	//hashtable_test(256);
	
	//HarpStr str(L"fuck");
	HarpStr str[4];
	harpstr hs = harpstr_new(L"FUNK", wcslen(L"FUNK"));
	str[0] = HarpStr(hs);
	str[0].Dump();
	str[0] = hs;
	str[0].Dump();
	str[0] = str[0];
	str[0].Dump();
	puts("====");

	str[1] = hs;
	str[1] = L"Funk";
	str[1] += L"Half";
	str[0].Dump();
	str[1].Dump();
	str[0] = str[1];
	hs->DecRC();

	HarpStr s(L"Shut");
	s.Dump();
	s += L"Down";
	s.Dump();
	return 0;
}