#ifndef __DEF_H__
#define __DEF_H__

#include "head.h"
#include "harpdata.h"
#include "infunc.h"

//using namespace std;


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
	SFnc, SFunc= SFnc,
	SInFnc,
};

#endif