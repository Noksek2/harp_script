#pragma warning(disable: 4146)
#include <h_mem.h>
#include <map>

void test_init() {
	setlocale(LC_ALL, "");
	//_wsetlocale(LC_ALL, L"korean");
	wcout.imbue(locale("")); 
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 
}

bool KeyComposer<harpstr>::Compare(const harpstr& s1, const harpstr& s2) {
	return s1->Compare(s2);
}
uint32_t KeyComposer<harpstr>::GetHash(const harpstr& s1) {
	return s1->hash;
}
void KeyComposer<harpstr>::Delete(const harpstr& s1) {
	free(s1);
	//s1 = NULL;
}
const harpstr& KeyComposer<harpstr>::Copy(const harpstr& s1) {
	return s1->Copy();
}

void harplist_test() {
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
}
void hashtable_test(uint32_t _CAPA) {
	const int CAPA = _CAPA;
	constexpr int BUFSIZE = 12;

	wstring buf;
	buf.resize(BUFSIZE);


	std::vector<int> rand_vec;
	rand_vec.reserve(CAPA);
	for (int i = 0; i < CAPA; i++) {
		rand_vec.push_back(rand() % CAPA);
	}

	clock_t t_s, t_e;
	uint8_t membuf[100];

	//{
	//	auto tb = MyHashTable<harpstr, int>::New(CAPA);
	//
	//	t_s = clock();
	//	for (volatile int i = 0; i < CAPA/2; i++) {
	//		_itow_s(rand_vec[i], &buf[0], BUFSIZE, 10);
	//		tb->emplace(harpstr_new(buf.c_str(), wcslen(buf.c_str())), (int)i);
	//	}
	//	t_e = clock();
	//	printf("insert %d\n", t_e - t_s);
	//
	//	tb->Dump();
	//
	//	
	//
	//	t_s = clock();
	//	volatile int failed = 0;
	//
	//
	//	for (volatile int i = 0; i < CAPA; i++) {
	//		_itow_s(rand_vec[i], &buf[0], BUFSIZE, 10);
	//		if (tb->find(harpstr_new(membuf, buf.c_str(), wcslen(buf.c_str()))) == NULL) failed++;
	//	}
	//	t_e = clock();
	//	printf("find %d failed/test (%d/%d)\n", t_e - t_s, failed, CAPA);
	//
	//
	//
	//	t_s = clock();
	//	for (volatile int i = 0; i < CAPA / 2; i++) {
	//		_itow_s(i, &buf[0], BUFSIZE, 10);
	//		tb->erase(harpstr_new(membuf, buf.c_str(), wcslen(buf.c_str())));
	//	}
	//	t_e = clock();
	//	printf("erase %d\n", t_e - t_s);
	//
	//	tb->Dump();
	//
	//
	//	t_s = clock();
	//	failed = 0u;
	//	for (volatile int i = 0; i < CAPA; i++) {
	//		_itow_s(i, &buf[0], BUFSIZE, 10);
	//		if (tb->find(harpstr_new(membuf, buf.c_str(), wcslen(buf.c_str()))) == NULL) failed++;
	//	}
	//	t_e = clock();
	//	printf("find %d failed/test (%d/%d)\n", t_e - t_s, failed, CAPA);
	//
	//
	//	tb->Dump();
	//
	//	delete tb;
	//
	//}
	auto shit = MyHashTable<std::wstring, int>::New(CAPA);
	
	puts("myhashtable");
	{
		t_s = clock();
		for (volatile int i = 0; i < CAPA/2; i++) {
			_itow_s(rand_vec[i], &buf[0], BUFSIZE, 10);
			shit->emplace(buf, (int)i);
			//printf("%d ", i);
		}
		t_e = clock();
		printf("insert %d\n", t_e - t_s);
		shit->Dump();
	
		t_s = clock();
		volatile int failed = 0;
		for (volatile int i = 0; i < CAPA; i++) {
			_itow_s(rand_vec[i], &buf[0], BUFSIZE, 10);
			if (shit->find(buf) == NULL) failed++;
		}
		t_e = clock();
		printf("find %d failed/test (%d/%d)\n", t_e - t_s, failed, CAPA);
	
		t_s = clock();
		for (volatile int i = 0; i < CAPA / 2; i++) {
			_itow_s(i, &buf[0], BUFSIZE, 10);
			shit->erase(buf);
		}
		t_e = clock();
		printf("erase %d\n", t_e - t_s);
	
		t_s = clock();
		failed = 0;
		for (volatile int i = 0; i < CAPA; i++) {
			_itow_s(i, &buf[0], BUFSIZE, 10);
			if (shit->find(buf) == NULL) failed++;
		}
		t_e = clock();
		printf("find %d failed/test (%d/%d)\n", t_e - t_s, failed, CAPA);
	
	
		shit->Dump();
		delete shit;
	
	}
	puts("unoredered_map");

	{
		std::unordered_map<std::wstring, int> ush;
		ush.reserve(CAPA/2);
		t_s = clock();
		for (volatile int i = 0; i < CAPA/2; i++) {
			_itow_s(rand_vec[i], &buf[0], BUFSIZE, 10);
			ush.emplace(buf, i);
		}
		t_e = clock();
		printf("insert %d\n", t_e - t_s);


		t_s = clock();
		volatile int failed = 0;
		for (volatile int i = 0; i < CAPA; i++) {
			_itow_s(rand_vec[i], &buf[0], BUFSIZE, 10);
			if (ush.find(buf) == ush.end()) failed++;
		}
		t_e = clock();
		printf("find %d (%d/%d)\n", t_e - t_s, failed, CAPA);

		t_s = clock();
		for (volatile int i = 0; i < CAPA / 2; i++) {
			_itow_s(i, &buf[0], BUFSIZE, 10);
			ush.erase(buf);
		}
		t_e = clock();
		printf("erase %d\n", t_e - t_s);

		t_s = clock();
		failed = 0;
		for (volatile int i = 0; i < CAPA; i++) {
			_itow_s(i, &buf[0], BUFSIZE, 10);
			if (ush.find(buf) == ush.end()) failed++;
		}
		t_e = clock();
		printf("find %d failed/test (%d/%d)\n", t_e - t_s, failed, CAPA);

	}

}

