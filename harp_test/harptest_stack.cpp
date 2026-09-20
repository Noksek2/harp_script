
#include <h_mem.h>

void main() {
	uint32_t test_cnt = _256MB;
	clock_t c_s, c_e;
	{
		c_s = clock();
		MyMemStack<int, _256MB>st;
		st.Init(_4KB);
		for (uint32_t i = 0; i < test_cnt; i++) {
			st.push(i);
		}
		c_e = clock();
		printf("memstack : %d ms", c_e - c_s);
	}
	Sleep(5000);
	{
		c_s = clock();
		MyStack<int, _256MB>st;
		st.Init(_4KB);
		for (volatile uint32_t i = 0; i < test_cnt; i++) {
			st.push(i);
		}
		c_e = clock();
		printf("stack : %d ms", c_e - c_s);
	}
	Sleep(5000);
	{
		c_s = clock();
		std::vector<int>st;
		st.reserve(_4KB);
		for (volatile uint32_t i = 0; i < test_cnt; i++) {
			st.emplace_back(i);
		}
		c_e = clock();
		printf("std::vector : %d ms", c_e - c_s);
	}
	Sleep(5000);
	return;
}