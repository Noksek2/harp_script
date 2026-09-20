/* harp script v0.2.0 */

#pragma once
#include "h_exec.h"
#include "h_errormsg.h"


#define NEXT t=tok.back().next() //tok.back()하면 되는데 1년전에는 그걸 모름. 병진
enum symtype{
	Sym1_Pack,
	Sym1_Modl,
	Sym1_Func,
	Sym1_Var,
	//Sym_Class,
	Sym2_Para
};
class SymbolTable;
struct SymbolData{
	harpstr name;
	SymbolData* Parent;
	MyStack<SymbolData> childs;
	SymbolTable* symtable;
	uint8_t sym1_typ;
	uint8_t sym2_typ;
	union {
		struct {
			int para_len;
			int frame_len;

		}f;
		struct {
			int member_cnt;

		}c;
	};
	void Init(harpstr*) {}
	void Delete(harpstr*) {}
};
class SymbolTable : public std::umap<std::wstring, SymbolData> {
//#define PARENT std::umap<std::wstring, SymbolData> 
public:
	SymbolData* Find(const wstring& str) {
		auto pair = this->find(str); 
		if (pair == this->end())return NULL;
		return &pair->second;
	}
#undef
};///*
//main:Ptr
//global[
//	"a":[sym_typ:modl name:"a" parent: path:"c:\\~~~" parent child:]
//	"b":[sym_typ:modl name:"b" ]
//  "c":[sym_typ:modl]
//  "math" []
//]
//*/
//template<typename K, typename V, const int CAPA>
//struct TinyHashTable {
//	K keymap[CAPA];
//	V keyval[CAPA];
//	uint32_t keyhash[CAPA];
//	void Init() {
//		memset(keymap, 0, sizeof(uint8_t) * CAPA);
//		memset(keyhash, 0, sizeof(uint32_t) * CAPA);
//	}
//
//	static inline uint32_t gethash(const wchar* name) {
//		uint32_t len = wcslen(name);
//		uint32_t hash;
//		for (uint32_t idx = 0; idx < len; idx++)
//		{
//			hash ^= name[idx];
//			hash *= 16777619; // FNV prime
//		}
//		if (hash == 0u) return 1u;
//		return hash;
//	}
//	void emplace(const wchar* name, uint8_t toktype) {
//		uint32_t kh = gethash(name);
//		const uint32_t o_idx = kh % 128;
//		uint32_t idx = o_idx;
//		while (idx < CAPA) {
//			if (keyhash[idx] == kh) {
//
//				idx++;
//			}
//		}
//		idx = 0;
//		while (idx < o_idx) {
//		}
//		Harp_assert(, "TinyHashTable emplace Error");
//	}
//	bool find(uint8_t& type, const wchar* name) {
//		name
//			uint32_t idx = find_idx();
//
//	}
//};
class KEY {
public:
	std::umap<std::wstring, ttype>typestr;
	//KeyFinder typestr;
	KEY() {
		//typestr.Init();
		/*함수*/
		typestr.emplace(L"func", _func);

		/*입출력*/
		typestr.emplace(L"print", _print);
		typestr.emplace(L"out", _out);
		typestr.emplace(L"put", _out);

		/*조건문*/
		typestr.emplace(L"if", _if);
		typestr.emplace(L"elif", _elif);
		typestr.emplace(L"else", _else);
		typestr.emplace(L"switch", _switch);
		typestr.emplace(L"case", _case);

		/*반복문*/
		typestr.emplace(L"while", _while);
		typestr.emplace(L"for", _for);
		typestr.emplace(L"loop", _loop);

		/*열거형 상수*/
		typestr.emplace(L"enum", _enum);
		//typestr.emplace(L"module", _module);

		/*분기*/
		typestr.emplace(L"break", _break);
		typestr.emplace(L"skip", _skip);
		typestr.emplace(L"redo", _skip);

		/*함수 리턴*/
		typestr.emplace(L"return", _return);

		/*내장 모듈 사용. 이름은 거창하네 신발*/
		typestr.emplace(L"use", _use);

		/*변수*/
		typestr.emplace(L"var", _var);

		/*파일 추가*/
		typestr.emplace(L"include", _include);

		/*아래는 기호들. 메모리 아깝다*/
		typestr.emplace(L"(", _Brack);
		typestr.emplace(L")", _Brackend);
		typestr.emplace(L"[", _Array);
		typestr.emplace(L"]", _Arrayend);
		typestr.emplace(L"\'", _Quot);
		typestr.emplace(L"\"", _Quots);
		typestr.emplace(L",", _Comma);
		typestr.emplace(L".", _Dot);
		typestr.emplace(L":", _Block);
		typestr.emplace(L";", _Blockend);

		typestr.emplace(L"+", _Plus);
		typestr.emplace(L"-", _Minus);
		typestr.emplace(L"*", _Multi);
		typestr.emplace(L"/", _Divi);
		typestr.emplace(L"%", _Mod);
		typestr.emplace(L"^", _Pow);
		typestr.emplace(L"<", _Less);
		typestr.emplace(L">", _Big);
		typestr.emplace(L"|", _Or);
		typestr.emplace(L"&", _And);
		typestr.emplace(L"#", _Hex);
		typestr.emplace(L"~", _Zusuk);
		typestr.emplace(L"!", _Not);
		typestr.emplace(L"=", _Equal);

		typestr.emplace(L"+=", _Add);
		typestr.emplace(L"-=", _Sub);
		typestr.emplace(L"%=", _Mod2);
		typestr.emplace(L"*=", _Mul);
		typestr.emplace(L"/=", _Div);
		typestr.emplace(L"^=", _Pow2);
		typestr.emplace(L"<=", _Lessis);
		typestr.emplace(L">=", _Bigis);
		typestr.emplace(L"==", _Equal2);
		typestr.emplace(L"!=", _Not2);
		typestr.emplace(L"=>", _default);
	}
	ttype findkey(const wstring& s) {//키워드 찾는 함수
		return (ttype)(typestr.find(s) != typestr.end() ? typestr[s] : _Ident);
	}
	ttype findgiho(const wstring& s) {//기호 찾는 함수
		return (ttype)(typestr.find(s) != typestr.end() ? typestr[s] : _Error);
	}
	//라고는 썼지만 두 함수 차이를 모르겠음.

	~KEY() {
		typestr.clear();//굳이 안 써도 됐는데
	}
}; extern KEY key;//이걸 전역변수로 선언해 버리는 클라스

/*토큰 구조체*/
struct token {
	wstring s;
	ttype typ;
	token() {}
	token(const wstring& ss, ttype t) :s(ss), typ(t) {}
};

class LEXER {
private:
public:
	int x, line, error;
	wstring source;
	wchar c;
	LEXER() {
		c = ' ';
		error = 0;
		x = 0, line = 1;
	}
	LEXER(wstring s) :source(s) { c = ' '; error = 0; x = 0, line = 1; }
	wchar get() {//문자 얻는데 이 짓거리를 해서 파싱이 느림.
		static char flag = 0;
		if (x >= (int)source.size())return None;
		else if (flag) { line++; flag = 0; }
		if (source[x] == '\n')flag = 1;
		return source[x++];
	}
	inline int range(wchar c, int a, int b) { //아니 매크로를 쓰라고 좀;
		return (c >= a && c <= b);
	}
	bool isunicode(wchar c) {//유니코드 검색
		return  (c == L'_'
				 || (c >= L'A' && c <= L'Z')
				 || (c >= L'a' && c <= L'z')
				 || (c >= '0' && c <= '9'));
			/*
			|| range(c, 0xAC00, 0xD7AF) || //한글
			range(c, 0x3040, 0x309F) || range(c, 0x30A0, 0x30FF) ||//일본어
			range(c, 0x4E00, 0x9FFF) || range(c, 0xF900, 0xFAFF))//한자*/
	}
	bool isident(wchar c) {//유니코드 검색
		return  (c == L'_'
				 || (c >= L'A' && c <= L'Z')
				 || (c >= L'a' && c <= L'z'));
		/*
		|| range(c, 0xAC00, 0xD7AF) || //한글
		range(c, 0x3040, 0x309F) || range(c, 0x30A0, 0x30FF) ||//일본어
		range(c, 0x4E00, 0x9FFF) || range(c, 0xF900, 0xFAFF))//한자*/
	}
	token next() {//다음 토큰 얻기
		wstring s = L"";
		string ss = "";
		if (error) {
			while (c != '\n' && c != None)c = get();
			error = 0;
		}
		while (iswspace(c))c = get();//문자가 공백이면 get()
		if (isident(c)) {//식별자 얻기
			for (; isunicode(c) || iswdigit(c); c = get()) {//식별자 얻음
				s += c;
			}
			return token(s, key.findkey(s));//식별자, 또는 예약어 반환
		}
		else if (iswdigit(c)) {//숫자 얻기
			for (; isunicode(c) || iswdigit(c); c = get()) {
				s += c;
			}
			if (c != '.') {//. 없으면 정수
				return token(s, _Int);
			}
			/*있으면 실수*/
			s += c;
			for (c = get(); iswdigit(c); c = get()) {
				s += c;
			}
			return token(s, _Num);
		}
		else if (c == '"') {//문자열 얻기
			for (c = get(); c != '"' && c; c = get()) {
				s += c;
			}
			c = get();
			return token(s, _Str);
		}
		else if (c == '\'') {//문자열 얻기 2
			for (c = get(); c != '\'' && c; c = get()) {
				s += c;//참고로 이스케이프 문자 안 만듬ㅋㅋㅋㅋ 엌ㅋㅋㅋ
			}
			c = get();
			return token(s, _Str);
		}
		else if (c == L'#') {//16진수 정수. 사용법은 대충 #00f00f 이런 식
			for (c = get(); iswxdigit(c); c = get())ss += (char)c;
			return token(to_wstring(strtol(ss.c_str(), NULL, 16)), _Int);
		}
		else if (c == L'~') {//주석이 ~ 이거밖에 없음. ~로 시작해서 ~로 끝냄
			c = get();
			while (c != L'~')c = get();
			c = get();
			return next();
		}
		else if (!c) { return token(L"", None); }//c가 null이면 공백 리턴
		else {
			/*기호 찾기*/
			s += c;
			c = get();
			s += c;
			if (key.findgiho(s) != _Error) {
				c = get(); return token(s, (ttype)key.typestr[s]);
			}
			s.pop_back();
			return token(s, key.findgiho(s));
		}
		return token(L"", None);
	}
	~LEXER() {
		source.clear();
	}
};
struct PolishStack{
#ifdef _DEBUG
	MyStack<optype>opstack;
#else
	MyStack<uint8_t>stack;
#endif
	optype asmgiho[TOK_MAX]; // TOKEN -> OP
	uint8_t oprank[OP_MAX]; // RANK OF OP
	void Init() {
		memset(asmgiho, 0, TOK_MAX);
		memset(oprank, 0, OP_MAX);

		asmgiho[ttype::_Plus] = optype::op_add;
		asmgiho[ttype::_Minus] = optype::op_sub;
		asmgiho[ttype::_Multi] = optype::op_mul;
		asmgiho[ttype::_Divi] = optype::op_div;
		asmgiho[ttype::_Pow] = optype::op_pow;
		asmgiho[ttype::_Mod] = optype::op_mod;
		asmgiho[ttype::_Big] = optype::op_gt;
		asmgiho[ttype::_Less] = optype::op_lt;
		asmgiho[ttype::_Bigis] = optype::op_gte;
		asmgiho[ttype::_Lessis] = optype::op_lte;
		asmgiho[ttype::_Equal2] = optype::op_eq;
		asmgiho[ttype::_Not2] = optype::op_neq;
		asmgiho[ttype::_And] = optype::op_and;
		asmgiho[ttype::_Or] = optype::op_or;

		//asmgiho[ttype::_Add]  = optype::op_add;
		//asmgiho[ttype::_Sub]  = optype::op_sub;
		//asmgiho[ttype::_Mul]  = optype::op_mul;
		//asmgiho[ttype::_Div]  = optype::op_div;
		//asmgiho[ttype::_Mod2] = optype::op_modeq;
		//asmgiho[ttype::_Pow2] = optype::op_poweq;

		/*연산자 우선순위*/
		oprank[optype::op_pow] = 1;
		oprank[optype::op_mul]
			= oprank[optype::op_div]
			= oprank[optype::op_mod]
			= 2;
		oprank[optype::op_add]
			= oprank[optype::op_sub]
			= 3;
		oprank[optype::op_gt]
			= oprank[optype::op_gte]
			= oprank[optype::op_lte]
			= oprank[optype::op_lt]
			= 4;
		oprank[optype::op_neq]
			= oprank[optype::op_eq]
			= 5;
		oprank[optype::op_and] = 6;
		oprank[optype::op_or] = 7;
	}
	void Clear() {
		opstack.m_len = 0u;
	}
	void push(ttype t) {
		if(t==_Brack){
			opstack.push(rank_brack);
			//oprank[opt] = t;
		}
		else {
			optype opt = (optype)asmgiho[(uint8_t)t];
			opstack.push(opt);
		}
	}
	optype pop() {
		return opstack.pop();
	}
	uint32_t isEmpty() {
		return opstack.m_len == 0;
	}
	bool isOp(ttype t) {
		return asmgiho[t] != 0;
	}

	const bool isRightOp(optype opt) const {
		if (opt == optype::op_pow) { return true; }
		if (opt == optype::op_min) { return true; }
		else return false;
	}
	const bool checkPop(optype tt, optype opt) const {//true면 계속 pop 진행
		if (tt == rank_brack) return false;
		if (isRightOp(opt)) { 
			if (oprank[tt] > oprank[opt]) return false;
		}//Right : + == **, ** == **
		//300 ** 500 ** --30(300 500 30 -- ** ** 
		else if (oprank[tt] >= oprank[opt]) return false;
		return true;
	}

	/*역폴란드 표기법 분석*/
	bool deletebrack() {
		while (opstack.m_len) {
			if(opstack.top() != rank_brack) 
				bytecode.push(opstack.pop(), 0);
			else {
				opstack.m_len--; return true;
			}
		}
		return false;
	}
	void tostack(ttype tt) {
		optype opt = asmgiho[(uint8_t)tt];
		optype c;
		while (opstack.m_len != 0) {
			c = opstack.top(); //* >= + -> 
			if (!checkPop(c, opt))break;
			bytecode.push(c, 0);
			opstack.m_len--;
		}
		opstack.push(opt);
	}
};
class COMPILE {//에러 클래스를 상속받음.
private:
	PolishStack polstack;
	vector<LEXER>tok;
	int nmod;
	uint32_t loopcount;
	uint32_t skippoint; //
	uint32_t nfunc; //n
	
	token t;
	
	
	//바이트 코드 변환 때문에 뭔가 더러움
	
public:
	COMPILE() {
		polstack.Init();
		exe.usefunc(L"basic");

		loopcount = 0;
		skippoint = 0U;
		nfunc = 0;
		nmod = -1;
		
	}
	int factor();//항
	void block() {//블록
		if (t.typ == _Block) {//만약 :로 시작하면
			NEXT;
			while (t.typ != _Blockend) {//;를 만날 때 까지
				state();//식 호출
			}
			NEXT;
			//이거만 보면 재귀하향분석법 같은데
			//아니라는게 함정
		}
		else state();
	}
	void funcdef();
	void term();
	void express();
	void state();
	void vardef(int);
	int identdef(int);
	void makearray();
	void ifblock() {//if문 분석... 노답;
		//바이트 코드 만드는 부분. 더러워서 패스
		//static MyStack<int>sp;
		constexpr int sp_maxcnt = 256;
		int sp[sp_maxcnt] = { 0 ,};
		int sp_idx = 0;

		NEXT;
		term();
		bytecode.push( op_ujmp, 0 );
		uint32_t s = bytecode.m_len - 1;
		block();//블록 호출
		bytecode.push( op_jmp, 0 );
		sp[sp_idx++] = bytecode.m_len - 1;
		bytecode[s].opr = bytecode.m_len;

		//elif 분석
		while (t.typ == _elif) {
			NEXT;
			term();
			bytecode.push( op_ujmp, 0 );
			s = bytecode.m_len - 1;
			block();//블록 호출
			bytecode.push( op_jmp, 0 );
			if (sp_idx >= sp_maxcnt) throw TOOMANY;
			sp[sp_idx++] = bytecode.m_len - 1;
			bytecode[s].opr = bytecode.m_len;
		}
		//else 분석
		if (t.typ == _else) {
			NEXT; block();//블록 호출
		}
		for (int i = 0; i < sp_idx; i++)
			bytecode[sp[i]].opr = bytecode.m_len;
	}
	void forblock() {
		//이걸 안 만들었네 허미
	}
	void whileblock() {//while 분석. 개 짧음
		NEXT;
		uint32_t oldpoint = skippoint;
		skippoint = bytecode.m_len;
		term();

		int s = bytecode.m_len;
		bytecode.push( op_ujmp );
		loopcount++;
		block();//블록 호출
		loopcount--;
		bytecode.push( op_jmp, skippoint );
		skippoint = oldpoint;
		bytecode[s].opr = bytecode.m_len;
	}
	void loopblock() {//이게 제일 더러움.
		int n, s = bytecode.m_len;
		uint8_t type;
		NEXT;
		n = exe.getlocalsym(t.s, nfunc);
		if (n == -1) {
			n = exe.getglobalsym(t.s);
			if (n == -1) {
				n = exe.pushsym(t.s, nfunc);
				type = op_lstore;
			}
			else type = op_gstore;
		}
		else type = op_lstore;
		NEXT;
		if (t.typ == _Equal) {
			NEXT;
			term();
		}
		else {
			bytecode.push( op_push );
		}
		bytecode.push( type, exe.symtable[n].mem );

		if (t.typ != _Comma)throw NOCOMMA;
		int oldpoint = skippoint;
		skippoint = bytecode.m_len;
		bytecode.push( (type == op_gstore ? op_gvar : op_lvar), exe.symtable[n].mem );
		NEXT;
		term();
		bytecode.push( op_lt );
		bytecode.push( op_ujmp, 0 );
		s = bytecode.m_len - 1;
		loopcount++;
		block();//블록 호출
		bytecode.push( op_push, 1 );
		bytecode.push( (type == op_gstore ? op_gvar : op_lvar), exe.symtable[n].mem );
		bytecode.push( op_add );
		bytecode.push( type, exe.symtable[n].mem );
		bytecode.push(op_jmp, skippoint);
		loopcount--;
		skippoint = oldpoint;
		bytecode[s].opr = bytecode.m_len;
		//뭐라고 쓴 건지 모르겠다 슈발
	}
	void compile(const wchar* dir) {//파일 불러오고 분석 시작
		ifstream in(dir);
		if (!in.is_open()) { ERRORMSG::puterror(dir, NOFILE, 0); /*throw NOFILE;*/return; }
		in.seekg(0, ios::end);
		string str;
		int size = (int)in.tellg();
		str.resize(size);
		in.seekg(0, ios::beg);
		in.read(&str[0], size);

		int len = MultiByteToWideChar(CP_UTF8, 0, &str[0], str.size(), NULL, NULL);
		wstring code(len, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], str.size(), &code[0], len);


		tok.emplace_back(code);
		NEXT;
		while (t.typ != None) {
			try {
				state(); //문장 분석
			}
			catch (errortype msg) { //예외 처리
				ERRORMSG::puterror(t.s, msg, tok.back().line);
				tok.back().error = 1;
				NEXT;
			}
		}
		tok.pop_back();
	}
	~COMPILE() {
		polstack.Clear();
		tok.clear();
	}
};