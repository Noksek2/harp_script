#include "h_exec.h"
/*여기가 실행 부분.*/
EXECUTE exe;
MemoryInfo g_meminfo;
harpobjpool* g_objpool;

/* 그냥 C 함수들 */
/*바이트 코드 분석하는 곳*/
static void Exec_func_call(harpfunc_p fn, harpdata* N, uint32_t para_len) {
	N[0] = fn(N, para_len);
	//N[0].SetErr(rte_call_failed);
}
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
		case op_push:case op_lit:
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
		//case op_lit:
		//	opstack.push(lit[opr]);
		//	break;
		case op_call: {
			Harp_assert(opstack.m_len > opr, "[RE] call error : dats_len is more than para_len");
			harpdata* pN;
			pN = &opstack[opstack.m_len - opr - 1];
			n1 = *pN; 
			switch (n1.GetType()) {
			case TInFnc: {
				uint64_t fnval = DECODE_VAL(n1);
				if (fnval < INFUNC_IDX_MAX) {
					const infunctype intyp = (infunctype)fnval;
					infunc_call(intyp, pN +1, opr);
				}
				else {
					harpfunc_p fnptr = (harpfunc_p)(void*)fnval;
					//funcmem.push(base);
					//funcmem.push(line);
					//base = nframe;
					Exec_func_call(fnptr, pN+1, opr);
					//line = symtable[n1.d.i].func - 1;
					//
					//nframe += symtable[n1.d.i].frame;
					//mem.resize(nframe);
				}

				break;
			}
			case TFnc: {
				break;
			}
			default:
				pN[0].SetErr(rte_infunc_unknown);
			}
			opstack.m_len -= opr;
			break;
		}
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
		case op_and:
		case op_or:	
		case op_pow: {
			opstack.m_len -= 1;
			uint32_t n = opstack.m_len;
			opstack[n - 1] = harpdata_calc(opstack[n - 1], opstack[n], op);
			break;
		}
		case op_not:
		case op_min: {
			uint32_t n = opstack.m_len;
			opstack[n - 1] = harpdata_calc_1(opstack[n - 1], op);
			break;
		}
		case op_print:
			for (uint32_t i = opstack.m_len - opr; i < opstack.m_len; i++) {
				opstack[i].Print();
			}
			opstack.m_len -= opr;
			cout << '\n';
			break;
		case op_out:
			for (uint32_t i = opstack.m_len - opr; i < opstack.m_len; i++) {
				opstack[i].Print();
			}
			opstack.m_len -= opr;
			break;
		}
		//pc 증가
		line++;
	}
}