
#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cmath>

#include <stdio.h>
#include <Windows.h>
#include <crtdbg.h>
#include <conio.h>
#include <time.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <random>

#define umap unordered_map 
using namespace std;
typedef wchar_t wchar;
typedef unsigned char uchar;

constexpr double MATH_PI = 3.1415926535897932384626;
constexpr double DX_PI = 3.14159265358979323846264338327;


constexpr uint64_t NAN_MARK = 0x7FF8000000000000ULL;
constexpr uint64_t SNAN_MARK = 0x7FF0000000000000ULL;
constexpr uint64_t INF_MARK = 0x7FF0000000000000ULL;
constexpr uint64_t BIT_15_MARK = 0x7FFF000000000000ULL;
constexpr uint64_t BIT_16_MARK = 0xFFFF000000000000ULL;
constexpr uint64_t NAN_ALL_MARK = 0x7FF7000000000000ULL;

constexpr uint64_t BIT_48 = 0x0000FFFFFFFFFFFFULL;
constexpr uint64_t INT48_MAX = 0x00007FFFFFFFFFFFULL;
constexpr uint64_t INT48_MIN = 0x0000800000000000ULL;

constexpr int64_t NORM_INT48_MAX = (int64_t)(INT48_MAX);
constexpr int64_t NORM_INT48_MIN = (int64_t)(INT48_MIN | (0xFFFFllu << 48llu));




enum {
	_4KB = 4 * 1024,
	_64KB = 64 * 1024,
	_4MB = 4 * 1024 * 1024,
	_16MB = 16 * 1024 * 1024,
	_64MB = 64 * 1024 * 1024,
	_128MB = _64MB * 2, 
	_256MB = _128MB * 2,
	_512MB = _256MB * 2,
	_1024MB = _512MB * 2,
	_1GB = _1024MB,
	MEM_MAX_DEFAULT = _16MB,//16MB
	RESERVE_MAX_DEFAULT = _1GB,//1GB
	
};

template<typename T>
static T* harp_malloc(size_t len) {
	return (T*)malloc(len * sizeof(T));
}
template<typename T>
static T* harp_calloc(size_t len) {
	return (T*)calloc(len, sizeof(T));
}
template<typename T>
static T* harp_realloc(T* dat, size_t len) {
	return (T*)realloc(dat, len * sizeof(T));
}
static void harp_free(void* ptr) {
	free(ptr);
}
static void Harp_assert(bool statement, const char* buf) {
#ifdef _DEBUG
	if (!statement) {
		puts(buf);
		__debugbreak();
	}

#endif
	if (!statement) {
		puts(buf);
		exit(1);
	}
}

static void Harp_assert_dbg(bool statement, const char* buf) {
#ifdef _DEBUG
	if (!statement) {
		puts(buf);
		__debugbreak();
	}

#endif
}



enum rterrtype {
	rte_unknown,
	rte_calc_failed, //[line, op_+]
	rte_func_para_no_match, //[(line), funcinfo(para_cnt, types=?)]
	rte_call_failed, //[line, wrong value]
	rte_infunc_unknown, // [line, infunctype]
};


static void* MEM_reserve(const uint64_t max_capa, const uint64_t T_sz, const uint64_t RESERVE_MAX) {
	uint64_t memsz = max_capa * T_sz;
	Harp_assert(memsz <= _1GB, "[ERROR] MEM_reserve cannot reserve more than 1GB");
	Harp_assert(memsz <= RESERVE_MAX, "[ERROR] MEM_reserve more than RESERVE_MAX");
	void* ptr = VirtualAlloc(
		NULL,
		memsz,
		MEM_RESERVE,
		PAGE_READWRITE
	);
	Harp_assert(ptr != NULL, "[ERROR] memory allocate failed - reserve");
	return ptr;
}
static void* MEM_commit(void* ptr, uint64_t len, uint64_t T_sz, const uint64_t RESERVE_MAX) {
	uint64_t memsz = len * T_sz;
	Harp_assert(memsz <= RESERVE_MAX, "[ERROR] MEM_reserve more than RESERVE_MAX");
	void* nptr = VirtualAlloc(
		ptr,
		memsz,
		MEM_COMMIT,
		PAGE_READWRITE
	);
	Harp_assert(nptr != NULL, "[ERROR] memory allocate failed - commit");
	return nptr;
}
static void MEM_free(void* ptr) {
	VirtualFree(ptr, 0, MEM_RELEASE);
}


template <typename T, const uint64_t RES_CNT_MAX = _64MB>

class MyMemStack {//성능 쓰레기같은 스택
private:
public:
	T* m_stack;
	uint32_t m_len;
	uint32_t m_capa;
	//uint32_t m_reserved;
	void Init(uint32_t m_capa_cnt = _4KB) {
		m_len = 0u;
		m_capa = max(m_capa_cnt, _4KB / sizeof(T));
		//Harp_assert(m_capa_cnt * sizeof(T) <= RES_MAX, "아 씨발년아");
		//m_reserved = m_capa_cnt;
		m_stack = (T*)MEM_reserve(RES_CNT_MAX, sizeof(T), RES_CNT_MAX * sizeof(T));
		m_stack = (T*)MEM_commit(m_stack, m_capa , sizeof(T), RES_CNT_MAX * sizeof(T));
	}
	inline void reserve(uint32_t re_size) {
		if (re_size > m_capa) {
			
			while (re_size > m_capa)
				m_capa <<= 1;
			if (m_capa > RES_CNT_MAX ) {
				m_capa = RES_CNT_MAX ;
			}
			m_stack = (T*)MEM_commit(m_stack, m_capa, sizeof(T), RES_CNT_MAX * sizeof(T));
		}
	}
	void push(const T& dat) {
		if (m_len >= m_capa) {
			reserve(m_len + 1);
		}
		m_stack[m_len++] = dat;
	}
	inline void resize(uint32_t re_size) {
		reserve(re_size);

		m_len = re_size;
	}
	inline T pop() {
		return m_stack[--m_len];
	}
	inline void onlypop() {
		m_len--;
	}
	inline T top() {
		return m_stack[m_len - 1];
	}
	inline T* back_ptr() {
		return &m_stack[m_len];
	}
	T& operator[] (const uint32_t idx) { return m_stack[idx]; }
	const T& operator[] (const uint32_t idx) const { return m_stack[idx]; }
	~MyMemStack() {
		if (m_stack) {
			MEM_free(m_stack);
			m_stack = NULL;
		}
	}
};

template <typename T, const uint64_t MEM_CNT_MAX = _64MB>
class MyStack {
private:
public:
	T* m_stack;
	uint32_t m_len;
	uint32_t m_capa;
	//uint32_t m_reserved;
	void Init(uint32_t _m_capa = _4KB) {
		m_len = 0u;
		m_capa = _m_capa;
		m_stack = harp_calloc<T>(m_capa);
		//m_stack = (T*)MEM_commit(m_stack, m_capa);
	}
	inline void reserve(uint32_t re_size) {
		if (re_size > m_capa) {
			while (re_size > m_capa)
				m_capa <<= 1;
			Harp_assert(m_capa <= MEM_CNT_MAX, "[ERROR] reserve more than MEM_MAX");
			T* newarr = harp_realloc<T>(m_stack, m_capa);
			Harp_assert(newarr != NULL, "[ERROR] realloc failed");
			m_stack = newarr;
		}

	}
	void push() {
		reserve(m_len + 1);
		m_len++;
	}
	void push(const T& dat) {
		if (m_len == m_capa) {
			reserve(m_len + 1);
		}
		m_stack[m_len++] = dat;
	}
	inline void resize(uint32_t re_size) {
		reserve(re_size);
		m_len = re_size;
	}

	inline const uint32_t size() { return m_len; }
	inline T pop() {
		return m_stack[--m_len];
	}
	inline void onlypop() {
		m_len--;
	}
	inline T top() {
		return m_stack[m_len - 1];
	}
	T& operator[] (const uint32_t idx) { return m_stack[idx]; }
	const T& operator[] (const uint32_t idx) const { return m_stack[idx]; }

	inline T* back_ptr() {
		return &m_stack[m_len];
	}
	~MyStack() {
		if (m_stack) {
			free(m_stack);
			m_stack = NULL;
		}
	}
};


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