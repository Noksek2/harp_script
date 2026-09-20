#ifndef __DEF_H__
#define __DEF_H__

#include "h_head.h"
#include "h_data.h"
#include "h_infunc.h"

//using namespace std;

struct RandomDevice {
	std::mt19937* gen;
	std::uniform_int_distribution<int64_t>* dis_int;
	std::uniform_int_distribution<double>* dis_float;
	RandomDevice() {
		std::random_device rd;
		gen = new std::mt19937(rd());
		dis_int = new std::uniform_int_distribution<int64_t>(NORM_INT48_MIN, NORM_INT48_MAX);
		dis_float = new std::uniform_int_distribution<double>(0.0, 1.0);
		constexpr auto d = NORM_INT48_MIN + NORM_INT48_MAX;
	}
	inline int64_t GetRand_i() {
		return (*dis_int)(gen);
	}
	inline double GetRand_f() {
		return (*dis_float)(gen);
	}
	inline int64_t GetRand_i(int64_t _max) {
		int64_t res = (*dis_int)(gen);
		return res % (_max + 1);
		//() % (_max - _min); 0 10 _min + _max+_min
	}
	inline int64_t GetRand_i(const int64_t _min, const int64_t _max) {
			int64_t res = (*dis_int)(gen);
			return _max + res % (_min - _max + 1);
	}
	~RandomDevice() {
		if(dis_float )delete dis_float;
		if(dis_int )delete dis_int;
		if(gen )delete gen;
		dis_float = NULL;
		dis_int = NULL;
		gen = NULL;
	}
	//140, 737, 488, 355, 327
	//-140,737,488,355,328
};


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
	_for,//for i inc 0, 1000, 3 
	_while,
	_loop,
	_break,
	_skip,
	_jump,
	_switch,
	_case,
	_func,
	_return,//
	_use,//use dxlib : dx
	_enum,
	_module,
	_include,
	_inc,
	_dec,

	k_load_dll,//load_dll 
	k_int,//int()
	k_float,//float()
	k_str,//str()
	k_task,//task

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

extern void HarpContext_Init();
extern void HarpContext_Delete();
extern RandomDevice* g_randdev;
#endif