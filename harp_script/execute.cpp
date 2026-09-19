#include "execute.h"
/*여기가 실행 부분.*/
EXECUTE exe;
MemoryInfo g_meminfo;
/* 그냥 C 함수들 */
/*바이트 코드 분석하는 곳*/
void EXECUTE::run() {
	uint8_t op;
	uint32_t opr;
	mem.resize(symtable[0].frame);
	nframe = symtable[0].frame;
	wstring c;
	while (line < bytecode.m_len) {
		op = bytecode[line].op;
		opr = bytecode[line].opr;
		switch (op) {
		case op_push:
			opstack.push(lit[opr]);
			break;
		case op_pop:
			opstack.m_len--;
			break;
		case op_jmp:
			line = opr - 1;
			break;
		case op_ujmp:
			if (!opstack.pop().isTrue())line = opr - 1;
			break;
		case op_at:
		//	n1 = opstack.pop();
		//	n2 = opstack.pop();
		//	switch (n2.type) {
		//	case Str:
		//		c = n2.s[n1.d.i];
		//		opstack.push(c);
		//		break;
		//	case Array:
		//		opstack.push(arraymem[n2.d.i][n1.d.i]);
		//		break;
		//	default:opstack.push(0);
		//	}
		//	break;
		case op_lit:
			opstack.push(lit[opr]);
			break;
		case op_call:
			Harp_assert(opstack.m_len >= opr, "[RE] call error : dats_len is more than para_len");
			//
			//for (uint32_t i = 0; i < opr; i++) {
			//	mstack.push(opstack.pop());
			//}
			//n1 = opstack.pop();
			//switch (n1.type) {
			//case Func:
			//	for (uint32_t i = 0; i < opr; i++) {
			//		opstack.push(mstack.m_stack[i]);
			//	}	mstack.m_len = 0;
			//	funcmem.push(base);
			//	funcmem.push(line);
			//	line = symtable[n1.d.i].func - 1;
			//	base = nframe;
			//	nframe += symtable[n1.d.i].frame;
			//	mem.resize(nframe);
			//	break;
			//case Infunc:
			//	infunc_call(symtable[n1.GetInt()])
			//	callinfunc(symtable[n1.d.i].func);
			//	mstack.m_len = 0;
			//	break;
			//}
			//break;
		case op_return:
			line = funcmem.pop();
			base = funcmem.pop();
			break;
		case op_pushfunc:
		//	n1 = (int)opr;
		//	n1.type = Func;
		//	mem[symtable[opr].mem] = n1;
		//	break;
		case op_pushinfunc:
		//	n1 = (int)opr;
		//	n1.type = Infunc;
		//	mem[symtable[opr].mem] = n1;
		//	break;
		case op_array:
		//	mvector.resize(opr);
		//	for (int i = opr - 1; i >= 0; i--) {
		//		mvector[i] = opstack.pop();
		//	}
		//	arraymem.emplace_back(mvector);
		//	n1.type = Array;
		//	n1.d.i = arraymem.size() - 1;
		//	opstack.push(n1);
		//	break;
		case op_lstore:
			mem[base + opr] = opstack.pop();
			break;
		case op_gstore:
			mem[opr] = opstack.pop();
			break;
		case op_lvar:
			opstack.push(mem[base + opr]);
			break;
		case op_gvar:
			opstack.push(mem[opr]);
			break;

			/*연산 부분*/
		case op_add:
		case op_sub:
		case op_mul:
		case op_div:
		case op_mod:
		case op_lt:	
		case op_lte:
		case op_gt:	
		case op_gte:
		case op_eq:	
		case op_neq:
		case op_not:
		case op_min:
		case op_and:
		case op_or:	
		case op_pow: {
			opstack.m_len -= 1;
			uint32_t n = opstack.m_len;
			opstack[n - 1] = harpdata_calc(opstack[n - 1], opstack[n], op);
			break;
		}
		case op_print:
			for (int i = opstack.m_len - opr; i < opstack.m_len; i++) {
				opstack[i].Print();
			}
			opstack.m_len -= opr;
			cout << '\n';
			break;
		case op_out:
			for (int i = opstack.m_len - opr; i < opstack.m_len; i++) {
				opstack[i].Print();
			}
			opstack.m_len -= opr;
			break;
		}
		//pc 증가
		line++;
	}
}