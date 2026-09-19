#pragma once
/*별거 없음*/
#include "def.h"
#include "memory.h"

// dats = stack[len]


struct codeset {
	uint32_t op : 8;
	uint32_t opr : 24;
	void Set(uint8_t _op, uint32_t _opr = 0) { op = _op; opr = _opr; }
};
class Bytecode : public MyStack<codeset> {
public:
	void push(uint8_t _op, uint32_t _opr = 0u) {
		codeset set;
		set.Set(_op, _opr);
		MyStack<codeset>::push(set);
	}
};

extern Bytecode bytecode;
struct symbol {//심볼 테이블 구조체
	uint8_t t;//변수 타입인 듯. (var, func )
	uint32_t mem;//index of vm memory where "var" uses
	uint32_t para;//count of paramater 
	uint32_t func;//함수의 번호 (infunc's enum no)
	uint32_t frame;// frame size of function 
	symbol() { t = SVar; mem = 0u; func = 0; frame = 0; para = 0; }
	symbol(uint8_t _t, uint32_t _mem, uint32_t _func, uint32_t _frame)
		:t(_t), mem(_mem), func(_func), frame(_frame), para(0) {
	}
};

class VMData {
	friend class EXECUTE;
	MyStack<harpdata>opstack;//오퍼랜드 스택인데 부르기 쪽팔림
	MyStack<harpdata>mem;//아마도 가상 메모리
public:
	MyStack<harpdata>lit;//리터럴
	VMData() {
		g_dll = 0;
	}
	template<typename T>
	inline void lit_push(const T& val) {
		harpdata h;
		h.SetInt(val);
		lit.push(h);
	}
	inline void lit_push(int64_t val) {
		harpdata h;
		h.byte = ENCODE_INT(val);
		lit.push(h);
	}
	inline void lit_push(double val) {
		harpdata h;
		h.f64 = val;
		lit.push(h);
	}
	inline void lit_push(const wchar_t* str, uint32_t len) {
		harpdata h;
		if (len < 6u) {
			//memcpy(h.s8 + 2, str, len);
			//h.byte = ENCODE_INSTR(h);
		}
		else {
			//h.f64 = val;
		}
		lit.push(h);
	}

	void endcompile() {
	}
	//아으 토나온다
};
class EXECUTE :public VMData {
protected:
	//VMData* vmdata;
	//MyStack<int>emptyarray;
	harpdata n1, n2;
	MyStack<uint32_t>funcmem;
	//MyStack<harpdata>mstack;
	MyStack<harpdata>mvector;
	uint32_t base;
	uint32_t line;
	uint32_t opr;
	uint32_t nframe;
	std::vector<std::umap<std::wstring, int>>symmap;//심볼 검색 테이블. 

	//Enumtable, add a enum's value in lit table, and add idx of littable in enummap
	std::umap<wstring, uint32_t>enummap;


	//vector<vector<var>>arraymem;//옛날에는 포인터 몰라서 이딴 방식으로 배열 구현함. 병진 하;
	friend class COMPILE;
public:
	std::vector<symbol>symtable;//심볼 테이블.. 인데 왜 2개나 있지
	EXECUTE()
		:line(0U),
		base(0U),
		opr(0U),
		nframe(0U)
	{
		cursorinfo.bVisible = 1;
		cursorinfo.dwSize = 1;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorinfo);
		srand((UINT)time(NULL));
		(void)rand();
		symmap.emplace_back();
		symtable.emplace_back(TFnc, 0, 0, 1);
	}
	void run();
	bool usefunc(const wstring& s);

	int pushfunc(const wstring& s) {
		symmap.emplace_back();
		symmap[0].emplace(s, (uint32_t)symtable.size());
		symtable.emplace_back(SFnc, symtable[0].frame, 0, 0);
		symtable[0].frame++;
		return symtable.size() - 1;
	}
	void pushinfunc(const wstring& s, uint32_t t) {
		symmap[0].emplace(s, (uint32_t)symtable.size());
		symtable.emplace_back(SInFnc, symtable[0].frame, t, 0);
		symtable[0].frame++;
		bytecode.push(op_pushinfunc, (uint32_t)symtable.size() - 1 );
	}
	template<typename T>
	void pushenum(const wstring& b, const T& val) {
		lit_push<T>(val);
		enummap.emplace(b, lit.m_len - 1);
	}
	void pushenum(const wstring& b, const int64_t val) {
		lit_push(val);
		enummap.emplace(b, lit.m_len - 1);
	}
	void pushenum(const wstring& b, const double val) {
		lit_push(val);
		enummap.emplace(b, lit.m_len - 1);
	}
	void pushenum(const wstring& b, const wstring& val) {
		lit_push(val.c_str(), val.size());
		enummap.emplace(b, lit.m_len - 1);
	}

	//아래는 심볼 찾거나 얻는 곳임. 아으 진짜 내가 썼지만 때리고 싶네
	bool findfsym(const wstring& s, int nfunc) {
		if (symmap[nfunc].find(s) != symmap[nfunc].end())return symmap[nfunc][s];
		return 0;
	}
	bool findsym(const wstring& s) {
		if (symmap[0].find(s) != symmap[0].end())return 1;
		return 0;
	}
	int getsym(const wstring& s) {
		if ((symmap.end() - 1)->find(s) != (symmap.end() - 1)->end())return (*(symmap.end() - 1))[s];
		else if (symmap[0].find(s) != symmap[0].end())return symmap[0][s];
		return -1;
	}
	int getlocalsym(const wstring& s, int nfunc) {
		if ((symmap.end() - 1)->find(s) != (symmap.end() - 1)->end())return (*(symmap.end() - 1))[s];
		return -1;
	}
	int getglobalsym(const wstring& s) {
		if (symmap[0].find(s) != symmap[0].end())return symmap[0][s];
		return -1;
	}
	int pushsym(const wstring& s, int nfunc) {
		symmap[symtable[nfunc].mem].emplace(s, symtable.size());
		symtable.emplace_back(SVar, symtable[nfunc].frame++, nfunc, 0);
		return symtable.size() - 1;
	}
	void callinfunc(infunctype infunc_type);
	~EXECUTE() { if (g_dll)FreeLibrary(g_dll); enummap.clear(); }
};
extern EXECUTE exe;