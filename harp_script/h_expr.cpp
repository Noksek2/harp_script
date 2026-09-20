#include "h_compile.h"
void COMPILE::makearray() {//배열 만드는 곳
	uint32_t arg = 0;
	do {
		NEXT;
		if (t.typ == _Array) {
			makearray();
			NEXT;
		}
		else if (t.typ == _Arrayend) {
			if (arg == 0) { bytecode.push( op_push, 0 ); arg++; break; }
			else throw BADARRAY;
		}
		else if (t.typ == _Comma)throw BADARRAY;
		else { polstack.push(_Brack); express(); polstack.deletebrack(); }
		arg++;
	} while (t.typ == _Comma);
	if (t.typ != _Arrayend)throw BADARRAY;
	bytecode.push( op_array, arg );
}
int COMPILE::factor() {//일차항 분석
	int buf = 0;
	wstring name;
l_redo:
	switch (t.typ) {
	case _Int:
		exe.lit_push((_wtoi64(t.s.c_str())));
		bytecode.push( op_lit, (uint32_t)exe.lit.size() - 1 );
		break;
	case _Num:
		exe.lit_push((_wtof(t.s.c_str())));
		bytecode.push( op_lit, (uint32_t)exe.lit.size() - 1 );
		break;
	case _Str:
		exe.lit_push(t.s.c_str(),(uint32_t)t.s.size());
		bytecode.push( op_lit, (uint32_t)exe.lit.size() - 1 );
		break;
	case _Plus:
		goto l_redo;
	case _Minus:
		NEXT;
		factor();
		bytecode.push( op_min );
		return 1;
	case _Not:
		NEXT;
		factor();
		bytecode.push( op_not );
		return 1;
	case _Brack:
		polstack.push(_Brack);
		NEXT;
		express();
		if (t.typ != _Brackend) { polstack.Clear(); throw NOBRACK; }
		polstack.deletebrack();
		break;
	case _Array:
		makearray();
		break;
	case _Ident:
		name = t.s;
		if (exe.enummap.find(name) != exe.enummap.end()) { 
			bytecode.push( op_lit, exe.enummap[name] );
			NEXT; return 1; 
		}
		buf = exe.getlocalsym(name, exe.symtable[nfunc].mem);
		if (buf != -1) {
			bytecode.push( op_lvar, exe.symtable[buf].mem );
		}
		else {
			buf = exe.getglobalsym(name);
			if (buf == -1)throw NOIDENT;
			bytecode.push( op_gvar, exe.symtable[buf].mem );
		}
		NEXT;
		if (t.typ == _Array || t.typ == _Brack) {
			while (t.typ == _Array) {
				NEXT;
				polstack.push(_Brack);
				express();
				if (t.typ != _Arrayend)throw NOARRAY;
				polstack.deletebrack();
				NEXT;
				bytecode.push( op_at, 0 );
			}
			if (t.typ == _Brack) {
				uint32_t cnt = 0; //para_cnt
				cnt = 0;
				do {
					NEXT;
					if (t.typ == _Brackend) { if (cnt == 0)break; else throw BADSTATE; }
					polstack.push(_Brack);
					express();
					polstack.deletebrack();
					cnt++;
				} while (t.typ == _Comma);
				NEXT;
				bytecode.push( op_call, cnt );
			}
		}
		return 1;
	}
	NEXT;
	return 1;
}
/*역폴란드 표기법 분석*/
void COMPILE::term() {
	express();
	while (!polstack.isEmpty()) {
		bytecode.push(polstack.pop(), 0);
	}
}
void COMPILE::express() {//표현식 검사
	while (factor()) {//항 연산자 항 연산자 .. 이런 순서로 분석됨

		if (!polstack.isOp(t.typ)) break;
		polstack.tostack(t.typ);
		NEXT;
	}
}