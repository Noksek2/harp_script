#ifndef __DEF_H__
#define __DEF_H__

#include "h_head.h"
#include "h_data.h"
#include "h_infunc.h"

//using namespace std;

struct RandomDevice {
	static std::mt19937& GetEngine() {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		return gen;
	}
	
	static inline int64_t GetRand_i(const int64_t _min, const int64_t _max) {
		std::uniform_int_distribution<int64_t> dis_int(NORM_INT48_MIN, NORM_INT48_MAX);
		return dis_int(GetEngine());
	}
	static inline double GetRand_f(double _min, double _max) {
		std::uniform_real_distribution<double> dis_f(_min, _max);
		return dis_f(GetEngine());
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
	//_jump,
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
	SPack,//Package = Directory 
	SModl,//Module = Source
	//SPara,
	SLocal,//Para C Local
	SMember,

	SFnc, SFunc= SFnc,
	SMethod,
	SInFnc, SInFunc= SInFnc,
	SClass,
	SEnum,
	SLabel,
	//SMacro,
	//SName,//익명
};
//Pack 1o : 1o에서 Pack 접근 가능. (1 -> Pack)
//class_D : 다른 클래스에서 
//1.class 2.Func 3. Method7.Modl_d 8 .Modl
// Pack <- Cox : Pack를 다른 클래스에서 참조 가능, 같은 클래스는 참조 불가능
//Pack <- Coo Foo Moo
//Modl <- Coo Foo Moo
//Class <- Coo Foo Moo
//Method <- C
//Class 1o 2o 
//Para(Method) 
//Method/Local/Variable부터는 접근 불가능

//메서드, 로컬변수, 멤버변수는 static(전역변수)가 아닌이상 외부에서 접근 불가능
//따라서 접근 가능한 녀석은 Pack, Modl, Enum, Class, Func등에 한정함
//나머지는 런타임 시간이나 실제 변수 멤버에서 접근할 수 있는 형태만 고려함.

/*
Pack
ㄴModl

Modl
ㄴFnc
ㄴInFnc
ㄴClass
(ㄴName)

Class
ㄴEnum
ㄴMethod=LocalFnc
ㄴMember
GlobalFnc
ㄴPara
ㄴLocal
InFnc
ㄴPara


*/
enum symtype2 : uint8_t{
	S2None,
	//Sym_Class,
	SPara,
	SLocal,

	//SFncMethod
	//SFnc
};

extern void HarpContext_Init();
extern void HarpContext_Delete();
extern RandomDevice* g_randdev;
#endif