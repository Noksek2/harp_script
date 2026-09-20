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


constexpr uint64_t NAN_MARK     = 0x7FF8000000000000ULL;
constexpr uint64_t SNAN_MARK    = 0x7FF0000000000000ULL;
constexpr uint64_t INF_MARK     = 0x7FF0000000000000ULL;
constexpr uint64_t BIT_15_MARK  = 0x7FFF000000000000ULL;
constexpr uint64_t BIT_16_MARK  = 0xFFFF000000000000ULL;
constexpr uint64_t NAN_ALL_MARK = 0x7FF7000000000000ULL;

constexpr uint64_t BIT_48 = 0x0000FFFFFFFFFFFFULL;
constexpr uint64_t INT48_MAX = 0x00007FFFFFFFFFFFULL;
constexpr uint64_t INT48_MIN = 0x0000800000000000ULL;

constexpr int64_t NORM_INT48_MAX = (int64_t)(INT48_MAX );
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

enum rterrtype {
	rte_unknown,
	rte_calc_failed, //[line, op_+]
	rte_func_para_no_match, //[(line), funcinfo(para_cnt, types=?)]
	rte_call_failed, //[line, wrong value]
	rte_infunc_unknown, // [line, infunctype]
};