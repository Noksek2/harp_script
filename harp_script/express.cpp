#include "compile.h"
void COMPILE::makearray() {//배열 만드는 곳
	uint32_t arg = 0;
	do {
		NEXT;
		if (t.typ == _Array) {
			makearray();
			NEXT;
		}
		else if (t.typ == _Arrayend) {
			if (arg == 0) { bytecode.push({ op_push, 0 }); arg++; break; }
			else throw BADARRAY;
		}
		else if (t.typ == _Comma)throw BADARRAY;
		else { gihostack.push(rank_brack); express(); deletebrack(); }
		arg++;
	} while (t.typ == _Comma);
	if (t.typ != _Arrayend)throw BADARRAY;
	bytecode.push({ op_array, arg });
}
int COMPILE::factor() {//일차항 분석
	int buf = 0;
	wstring name;
	switch (t.typ) {
	case _Int:
		exe.lit_push((_wtoi64(t.s.c_str())));
		bytecode.push({ op_lit, (uint32_t)exe.lit.size() - 1 });
		break;
	case _Num:
		exe.lit_push((_wtof(t.s.c_str())));
		bytecode.push({ op_lit, (uint32_t)exe.lit.size() - 1 });
		break;
	case _Str:
		exe.lit_push(t.s.c_str(),(uint32_t)t.s.size());
		bytecode.push({ op_lit, (uint32_t)exe.lit.size() - 1 });
		break;
	case _Plus:
		NEXT;
		factor();
		return 1;
	case _Minus:
		NEXT;
		factor();
		bytecode.push({ op_min });
		return 1;
	case _Not:
		NEXT;
		factor();
		bytecode.push({ op_not });
		return 1;
	case _Brack:
		gihostack.push(rank_brack);
		NEXT;
		express();
		if (t.typ != _Brackend) { gihostack.m_len=0; throw NOBRACK; }
		deletebrack();
		break;
	case _Array:
		makearray();
		break;
	case _Ident:
		name = t.s;
		if (exe.enummap.find(name) != exe.enummap.end()) { 
			bytecode.push({ op_lit, exe.enummap[name] });
			NEXT; return 1; 
		}
		buf = exe.getlocalsym(name, exe.symtable[nfunc].mem);
		if (buf != -1) {
			bytecode.push({ op_lvar, exe.symtable[buf].mem });
		}
		else {
			buf = exe.getglobalsym(name);
			if (buf == -1)throw NOIDENT;
			bytecode.push({ op_gvar, exe.symtable[buf].mem });
		}
		NEXT;
		if (t.typ == _Array || t.typ == _Brack) {
			while (t.typ == _Array) {
				NEXT;
				gihostack.push(rank_brack);
				express();
				if (t.typ != _Arrayend)throw NOARRAY;
				deletebrack();
				NEXT;
				bytecode.push({ op_at, 0 });
			}
			if (t.typ == _Brack) {
				uint32_t cnt = 0; //para_cnt
				cnt = 0;
				do {
					NEXT;
					if (t.typ == _Brackend) { if (cnt == 0)break; else throw BADSTATE; }
					gihostack.push(rank_brack);
					express();
					deletebrack();
					cnt++;
				} while (t.typ == _Comma);
				NEXT;
				bytecode.push({ op_call, cnt });
			}
		}
		return 1;
	}
	NEXT;
	return 1;
}

/*역폴란드 표기법 분석*/
void COMPILE::deletebrack(){
	while (gihostack.top() != rank_brack) {
		bytecode.push({ gihostack.pop(), 0 });
	}
	gihostack.m_len--;
}
void COMPILE::term() {
	express();
	while (gihostack.m_len!=0) {
		bytecode.push({ gihostack.pop(), 0 });
	}
}
void COMPILE::tostack(uint8_t i) {
	uint8_t c;
	while (gihostack.m_len != 0) {
		c = gihostack.top();
		if (
			c == rank_brack
			|| gihorank[c] > gihorank[i]
			)break;
		bytecode.push({ c, 0 });
		gihostack.m_len--;
	}
	gihostack.push(i);
}
void COMPILE::express() {//표현식 검사
	while (factor()) {//항 연산자 항 연산자 .. 이런 순서로 분석됨
		if (!asmgiho[t.typ])break;
		tostack(asmgiho[t.typ]);
		NEXT;
	}
}