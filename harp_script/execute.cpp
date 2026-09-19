#include "execute.h"
/*여기가 실행 부분.*/
EXECUTE exe;
MemoryInfo g_meminfo;
/* 그냥 C 함수들 */
/*바이트 코드 분석하는 곳*/
void EXECUTE::run() {
	uint8_t op;
	mem.resize(symtable[0].frame);
	nframe = symtable[0].frame;
	wstring c;
	while (line < bytecode.m_len) {
		op = bytecode[line].op;
		opr = bytecode[line].opr;
		switch (op) {
		case op_push:
			opstack.push((int)opr);
			break;
		case op_pop:
			opstack.m_len--;
			break;
		case op_jmp:
			line = opr - 1;
			break;
		case op_ujmp:
			if (!opstack.pop().True())line = opr - 1;
			break;
		case op_at:
			n1 = opstack.pop();
			n2 = opstack.pop();
			switch (n2.type) {
			case Str:
				c = n2.s[n1.d.i];
				opstack.push(c);
				break;
			case Array:
				opstack.push(arraymem[n2.d.i][n1.d.i]);
				break;
			default:opstack.push(0);
			}
			break;
		case op::op_lit:
			opstack.push(lit[opr]);
			break;
		case op::op_call:
			for (uint32_t i = 0; i < opr; i++) {
				mstack.push(opstack.pop());
			}
			n1 = opstack.pop();
			switch (n1.type) {
			case Func:
				for (uint32_t i = 0; i < opr; i++) {
					opstack.push(mstack.m_stack[i]);
				}	mstack.m_len = 0;
				funcmem.push(base);
				funcmem.push(line);
				line = symtable[n1.d.i].func - 1;
				base = nframe;
				nframe += symtable[n1.d.i].frame;
				mem.resize(nframe);
				break;
			case Infunc:
				callinfunc(symtable[n1.d.i].func);
				mstack.m_len = 0;
				break;
			}
			break;
		case op_return:
			line = funcmem.pop();
			base = funcmem.pop();
			break;
		case op_pushfunc:
			n1 = (int)opr;
			n1.type = Func;
			mem[symtable[opr].mem] = n1;
			break;
		case op_pushinfunc:
			n1 = (int)opr;
			n1.type = Infunc;
			mem[symtable[opr].mem] = n1;
			break;
		case op_array:
			mvector.resize(opr);
			for (int i = opr - 1; i >= 0; i--) {
				mvector[i] = opstack.pop();
			}
			arraymem.emplace_back(mvector);
			n1.type = Array;
			n1.d.i = arraymem.size() - 1;
			opstack.push(n1);
			break;
		case op::op_lstore:
			mem[base + opr] = opstack.pop();
			break;
		case op::op_gstore:
			mem[opr] = opstack.pop();
			break;
		case op::op_lvar:
			opstack.push(mem[base + opr]);
			break;
		case op::op_gvar:
			opstack.push(mem[opr]);
			break;

			/*연산 부분*/
		case op::op_plus:	n2 = opstack.pop(); n1 = opstack.pop(); opstack.push(n1 + n2); break;
		case op::op_minus:	n2 = opstack.pop(); n1 = opstack.pop(); opstack.push(n1 - n2); break;
		case op::op_multi:	n2 = opstack.pop(); n1 = opstack.pop(); opstack.push(n1 * n2); break;
		case op::op_divi:	n2 = opstack.pop(); n1 = opstack.pop(); opstack.push(n1 / n2); break;
		case op::op_mod:	n2 = opstack.pop(); n1 = opstack.pop(); opstack.push(n1 % n2); break;
		case op::op_less:	n2 = opstack.pop(); n1 = opstack.pop(); opstack.push(n1 < n2); break;
		case op::op_less2:	n2 = opstack.pop(); n1 = opstack.pop(); opstack.push(n1 <= n2); break;
		case op::op_big:	n2 = opstack.pop(); n1 = opstack.pop(); opstack.push(n1 > n2); break;
		case op::op_big2:	n2 = opstack.pop(); n1 = opstack.pop(); opstack.push(n1 >= n2); break;
		case op::op_equal2:	n2 = opstack.pop(); n1 = opstack.pop(); opstack.push(n1 == n2); break;
		case op::op_not2:	n2 = opstack.pop(); n1 = opstack.pop(); opstack.push(n1 != n2); break;
		case op::op_not:	n2 = opstack.pop(); opstack.push(n2.not1()); break;
		case op::op_min:	n2 = opstack.pop(); opstack.push(n2.minus1()); break;
		case op::op_and:	n2 = opstack.pop(); n1 = opstack.pop(); opstack.push(n1 && n2); break;
		case op::op_or:		n2 = opstack.pop(); n1 = opstack.pop(); opstack.push(n1 || n2); break;
		case op::op_pow:	n2 = opstack.pop(); n1 = opstack.pop(); opstack.push(n1.poww(n2)); break;

			/*출력하는 부분. 그냥 opstack.end()하면 되는데 병진*/
		case op::op_print:
			for (int i = opstack.m_len - opr; i < opstack.m_len; i++) {
				opstack[i].out();
			}
			opstack.m_len -= opr;
			cout << "\n";
			break;
		case op::op_out:
			for (int i = opstack.m_len - opr; i < opstack.m_len; i++) {
				opstack[i].out();
			}
			opstack.m_len -= opr;
			break;
		}
		//pc 증가
		line++;
	}
}