
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