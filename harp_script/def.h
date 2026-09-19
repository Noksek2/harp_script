#ifndef __DEF_H__
#define __DEF_H__

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <cmath>

#include <stdio.h>
#include <Windows.h>
#include <crtdbg.h>
#include <conio.h>
#include <time.h>
#include "infunc.h"
#include "harpdata.h"
using namespace std;
enum {
	_4KB = 4 * 1024,
	_64KB = 64 * 1024,
	_4MB = 4 * 1024 * 1024,
	_64MB = 64 * 1024 * 1024,
	MEM_MAX = _4MB,
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


typedef wchar_t wchar;
/*
class var {

	void out() {
		if (type == Int) {
			cout << d.i;
		}
		else if (type == Num) {
			cout << d.n;
		}
		else if (type == Str) {
			wcout << s;
		}
	}
	void print() {
		if (type == Int) {
			cout << d.i<<"\n";
		}
		else if (type == Num) {
			cout << d.n << "\n";
		}
		else if (type == Str) {
			wcout << s << "\n";
		}
	}


	wstring mulstr(wstring s,long long n) {
		wstring a = L"";
		for(int i=0;i<(int)n;i++)a += s;
		return a;
	}

	var minus1() {
		if (type == Int) {
			return -d.i;
		}
		else if (type == Num) {
			return -d.n;
		}
		return 0;
	}
	var not1() {
		if (type == Int) {
			return !d.i;
		}
		else if (type == Num) {
			return !d.n;
		}
		else if (type == Str) {
			return s == L"";
		}
		return 0;
	}
	var poww(var &b) {
		if (type == b.type) {
			if (b.type == Int)return int(pow(d.i , b.d.i));
			else if (b.type == Num)return pow(d.n , b.d.n);
		}
		else if (type == Num)return pow(d.n , (double)b.d.i);
		else if (b.type == Num)return pow((double)d.i , b.d.n);
		return 0;
	}
	int True() {
		if (type == Int) {
			return d.i==1;
		}
		else if (type == Num) {
			return d.n==1.0;
		}
		else if (type == Str) {
			return s == L"";
		}
		return 0;
	}
	
};*/

enum ttype : uint8_t{
	None,
	_Int,
	_Num,
	_Str,
	_Ident,

	_var,
	_print,
	_out,
	_if,
	_elif,
	_else,
	_for,
	_while,
	_loop,
	_break,
	_skip,
	_jump,
	_switch,
	_case,
	_func,
	_return,
	_use,
	_enum,
	_module,
	_include,

	_Brack,	//(
	_Brackend,//)
	_Array,	//[
	_Arrayend,//]
	_Dot,	//.
	_Comma,	//,
	_Blockend,	//;
	_Block,	// :
	_Colon2, //::

	_Quot,	//'
	_Quots,	//"

	_Plus,	//+
	_Minus,	//-
	_Multi,	//*
	_Divi,	// /
	_Pow,	//^
	_Mod,	//%
	_Less,	//<
	_Big,	//>
	_And,	//&
	_Or,	//|
	_Not,	//!
	_Zusuk,	//~
	_Equal,	//=
	_Hex,	//#

	/*_Or1,	// or
	_And1,	// and
	_Xor,	// xor
	_Not1,	// not
	_Rshift,// lshift >>
	_Lshift,// rshift <<
	*/
	_Add,	//+=
	_Sub,	//-=
	_Mul,	//*=
	_Div,	// /=
	_Pow2,	// ^=
	_Mod2,	//%=
	_Not2,	//!=
	_Equal2,//==
	_Lessis,//<=
	_Bigis,//>=
	_default,//=>
	_Atsign,//@
	_Zusuk2,//	//
	_Error,

	TOK_MAX=128,
};
enum symtype : uint8_t {
	SVar,
	SFnc,
	SInFnc,
};

#endif