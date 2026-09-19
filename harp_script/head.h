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
	MEM_MAX = _16MB,
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



enum rterrtype {
	rte_unknown,
	rte_calc_failed, //[line, op_+]
	rte_func_para_no_match, //[(line), funcinfo(para_cnt, types=?)]
	rte_call_failed, //[line, wrong value]
	rte_infunc_unknown, // [line, infunctype]
};