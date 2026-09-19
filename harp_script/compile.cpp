#include "compile.h"
KEY key;
void COMPILE::funcdef() {//함수 쓰지 마셈. 오류 걸림
	uint32_t fndef_idx;
	NEXT;
	if (t.typ != _Ident)throw BADIDENT;
	if (exe.findsym(t.s))throw OVERIDENT;
	exe.pushfunc(t.s);

	nfunc = (uint32_t)exe.symtable.size() - 1;
	bytecode.push({ op_pushfunc, nfunc });
	fndef_idx = bytecode.m_len;
	bytecode.push({ op_jmp, 0 });

	exe.symtable[nfunc].func = bytecode.m_len;
	NEXT;
	if (t.typ != _Brack)throw BADFUNC;
	do {
		NEXT;
		if (t.typ == _Brackend) {
			if (exe.symtable[nfunc].para == 0U)break;
			else throw BADFUNC; 
		}
		vardef(1);
		exe.symtable[nfunc].para++;
	} while (t.typ == _Comma);
	if (t.typ != _Brackend)throw BADFUNC;
	NEXT;
	block();
	nfunc = 0;
	bytecode.push({ op_push, 1U });
	bytecode.push({ op_return });
	bytecode[fndef_idx].opr = bytecode.m_len;
}
void COMPILE::vardef(int i = 0) {//var 분석
	wstring buf;
	int mem;
	if (i) {
		buf = t.s;
		if (t.typ != _Ident)throw BADIDENT;
		else if (exe.findfsym(buf,exe.symtable[nfunc].mem))throw OVERIDENT;
		NEXT;
		mem = exe.pushsym(buf, nfunc);
		bytecode.push({ op_lstore, exe.symtable[mem].mem });
		return;
	}
	do {
		NEXT;
		buf = t.s;
		if (t.typ!=_Ident)throw BADIDENT;
		else if (exe.findfsym(buf, exe.symtable[nfunc].mem))throw OVERIDENT;
		NEXT;
		mem = exe.pushsym(buf, nfunc);
		if (t.typ == _Equal) {
			NEXT;
			term();
		}
		else {
			bytecode.push({ op_push, 0 });
		}
		bytecode.push({ op_lstore, exe.symtable[mem].mem });
	} while (t.typ == _Comma);
}
int COMPILE::identdef(int i = 0) {//식별자 호출 또는 대입 검사.
	wstring buf = t.s;
	int mem;
	if (i) {
		buf = t.s;
		if (t.typ != _Ident)throw BADIDENT;
		else if (exe.findfsym(buf, exe.symtable[nfunc].mem))throw OVERIDENT;
		NEXT;
		mem = exe.pushsym(buf, nfunc);
		if (t.typ == _Equal) {
			NEXT;
			term();
		}
		else {
			bytecode.push({ op_push, 0 });
		}
		bytecode.push({ op_lstore, exe.symtable[mem].mem });
		return 1;
	}
	int n;
	uint32_t arg = 0;
	uint8_t typ;
	n = exe.getlocalsym(buf, nfunc);
	if (n == -1) { n = exe.getglobalsym(buf); if (n == -1) return 0; typ = op_gstore; }
	else typ = op_lstore;
	NEXT;
	if (t.typ == _Array) {
		bytecode.push({ (typ == op_gstore ? op_gvar : op_lvar), exe.symtable[n].mem });
		while (t.typ == _Array) {
			NEXT;
			term();
			if (t.typ != _Arrayend)throw BADARRAY;
			bytecode.push({ op_set });
			NEXT;
		}
	
		if (t.typ == _Equal) {
			NEXT;
			term();
		}
	}
	else if (t.typ == _Brack) {
		bytecode.push({ (typ == op_gstore ? op_gvar : op_lvar), exe.symtable[n].mem });
		do{
			NEXT;
			if (t.typ == _Brackend) {
				if (arg == 0)break;
				else throw BADSTATE;
			}
			arg++;
			term();
		} while (t.typ == _Comma);
		if (t.typ != _Brackend)throw BADSTATE;
		NEXT;
		bytecode.push({ op_call, arg });
		bytecode.push({ op_pop });
		
	}
	else if (t.typ == _Equal) {
		NEXT;
		term();
		bytecode.push({ typ, exe.symtable[n].mem });
	}
	else {
		switch (t.typ) {
		case _Add:
			bytecode.push({ (typ == op_gstore ? op_gvar : op_lvar), exe.symtable[n].mem });
			NEXT;
			term();
			bytecode.push({ op_plus });
			bytecode.push({ typ, exe.symtable[n].mem });
			break;
		case _Sub:
			bytecode.push({ (typ == op_gstore ? op_gvar : op_lvar), exe.symtable[n].mem });
			NEXT;
			term();
			bytecode.push({ op_minus });
			bytecode.push({ typ, exe.symtable[n].mem });
			break;
		case _Mul:
			bytecode.push({ (typ == op_gstore ? op_gvar : op_lvar), exe.symtable[n].mem });
			NEXT;
			term();
			bytecode.push({ op_multi });
			bytecode.push({ typ, exe.symtable[n].mem });
			break;
		case _Div:
			bytecode.push({ (typ == op_gstore ? op_gvar : op_lvar), exe.symtable[n].mem });
			NEXT;
			term();
			bytecode.push({ op_divi });
			bytecode.push({ typ, exe.symtable[n].mem });
			break;
		case op_modis:
			bytecode.push({ (typ == op_gstore ? op_gvar : op_lvar), exe.symtable[n].mem });
			NEXT;
			term();
			bytecode.push({ op_mod });
			bytecode.push({ typ, exe.symtable[n].mem });
			break;
		case op_powis:
			bytecode.push({ (typ == op_gstore ? op_gvar : op_lvar), exe.symtable[n].mem });
			NEXT;
			term();
			bytecode.push({ op_pow });
			bytecode.push({ typ, exe.symtable[n].mem });
			break;
		}
	}
	return 1;
}
void COMPILE::state() {//문장 분석
	uint32_t arg = 0;
	double d=0.0;
	char enumt=_Int;
	wstring b;
	switch (t.typ) {
	case _return://리턴문 분석
		NEXT;
		term();
		if (nfunc > 0) {
			bytecode.push({ op_return });
		}
		else throw BADRETURN;
		break;
	case _enum: //상수 분석
		NEXT;
		d = 0.0;
		if (t.typ != _Block)throw NOBLOCK;
		if (t.typ == _Blockend) { NEXT; break; }
		do {
			NEXT;
			if (t.typ != _Ident)throw BADIDENT;
			b = t.s;
			NEXT;
			if (t.typ != _Equal) {
				if (enumt == Int) {
					exe.lit.emplace_back((int)d);
				}
				else {
					exe.lit.emplace_back(d);
				}
				exe.enummap.emplace(b, exe.lit.size() - 1);
				d += 1.0;
			}
			else {
				NEXT;
				switch (t.typ) {
				case _Int:
					enumt = Int;
					d = stoi(t.s);
					exe.lit.emplace_back((int)d);
					exe.enummap.emplace(b, exe.lit.size()-1);
					d += 1.0;
					break;
				case _Num:
					enumt = Num;
					d = stod(t.s);
					exe.lit.emplace_back(d);
					exe.enummap.emplace(b, exe.lit.size() - 1);
					d += 1.0;
					break;
				case _Str:
					exe.lit.emplace_back(t.s);
					exe.enummap.emplace(b, exe.lit.size() - 1);
					break;
				default:throw BADENUM;
				}
				NEXT;
			}
		} while (t.typ == _Comma);
		if (t.typ != _Blockend)throw BLOCKOPEN;
		NEXT;
		break;
	case _var://var 분석
		vardef();
		break;
	case _Ident://식별자 분석
		if (!identdef()) {
			identdef(1);
		}
		break;
	case _print://출력문 분석
		do {
			NEXT;
			if (t.typ == _Comma)throw BADPRINT;
			term();
			arg++;
		} while (t.typ == _Comma);
		bytecode.push({ op_print, arg });
		break;
	case _out://출력문 분석
		do {
			NEXT;
			if (t.typ == _Comma)throw BADPRINT;
			term();
			arg++;
		} while (t.typ == _Comma);
		bytecode.push({ op_out, arg });
		break;
	case _include://include 문
		throw NOTYET;
			/*
		arg = blockcnt;
		NEXT;
		if (t.typ != _Str)throw BADINCLUDE;
		//compile(t.s.c_str());
		blockcnt = arg;
		NEXT;*/
		break;
	case _if://if문 분석
		ifblock();
		break;
		case _while://while문 분석
		whileblock();
		break;
	case _loop://loop문 분석
		loopblock();
		break;
	case _use://모듈 사용
		do {
			NEXT;
			if (!exe.usefunc(t.s))throw NOMODULE;
			NEXT;
		} while (t.typ == _Comma);
		break;
	case _Error:
		throw errortype::NOTOKEN;
		break;
	default:
		throw errortype::BADSTATE;
		break;

	/*아래는 오류 또는 미구현
	case _func:
		funcdef();//함수 쓰지 마셈. 오류 걸림
		break;
	case _skip://이거 안 됨. ㅅㄱ
		bytecode.push({op_jmp, skippoint);
		NEXT;
		break;
	case _for://이거 안 됨. ㅅㄱ
		forblock();
		break;*/
	
	}
}