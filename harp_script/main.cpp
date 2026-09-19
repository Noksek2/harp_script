/*
Toy script by Noksek2

lol

*/
#include "compile.h"
#include <time.h>
Bytecode bytecode;//아마도 바이트코드
void PrintBytecode() {
	const char* opp[100] = { 0 };
	opp[op_push] = "PUSH";
	opp[op_pop] = "POP";
	opp[op_call] = "CALL";
	opp[op_incall] = "INCALL";
	opp[op_lit] = "LIT";
	opp[op_lvar] = "LVAR";
	opp[op_gvar] = "GVAR";
	opp[op_lstore] = "LSTORE";
	opp[op_gstore] = "GSTORE";
	opp[op_pushinfunc] = "PUSHINFUNC";
	opp[op_pushfunc] = "PUSHFUNC";
	//opp[op_pushprev,

	opp[op_set] = "SET";
	opp[op_sets] = "SETS";
	opp[op_at] = "AT";
	opp[op_array] = "ARRAY";
	opp[op_jmp] = "JMP";
	opp[op_ujmp] = "UJMP";
	opp[op_return] = "RETURN";

	opp[op_or] = "OR";
	opp[op_and] = "AND";
	opp[op_not2] = "NOT2";
	opp[op_equal2] = "EQUAL2";
	opp[op_big2] = "BIG2";
	opp[op_big] = "BIG";
	opp[op_less2] = "LESS2";
	opp[op_less] = "LESS";
	opp[op_plus] = "PLUS";
	opp[op_minus] = "MINUS";
	opp[op_multi] = "MULTI";
	opp[op_divi] = "DIVI";
	opp[op_mod] = "MOD";
	opp[op_pow] = "POW";
	opp[op_min] = "MIN";
	opp[op_not] = "NOT";
	opp[op_print] = "PRINT";
	opp[op_out] = "OUT";

	opp[op_add] = "ADD";
	opp[op_sub] = "SUB";
	opp[op_mul] = "MUL";
	opp[op_div] = "DIV";
	opp[op_modis] = "MODIS";
	opp[op_powis] = "POWIS";
	opp[op_ang] = "ANG";
	ofstream out("bytecode.txt");
	out << "========symtable========\n";
	out << "no	type	mem	func	frame\n";
	char buf[][7] = {"Var","Func","InFunc"};
	int buf_no;
	for (int i = 0; i < exe.symtable.size(); i++) {
		switch (exe.symtable[i].t) {
		case Var: buf_no = 0; break;
		case Func:buf_no = 1; break;
		default: buf_no = 2; break;
		}
		out << i << '\t' << buf[buf_no] << '\t' << exe.symtable[i].mem << '\t' 
			<< exe.symtable[i].func << "\t" << exe.symtable[i].frame << "\n";
	}
	out << "\n\n\n========bytecode========\n";
	for (int i = 0; i < bytecode.m_len; i++) {
		out << opp[bytecode[i].op] << " " << bytecode[i].opr << "\n";
	}
}
void check_argv(COMPILE* compile,int argc, wchar** argv, bool& b_outfile) {
	//argc >= 2
	wchar** const argv_org = argv;
	wchar** const argv_end = argv + argc;
	if (argc == 2) compile->compile(argv[1]);
	while (argv != argv_end) {
		argv++;
		/*
		if (wcscmp(*argv, L"-o") == 0) {

		}
		else if (wcscmp(*argv, L"-o") == 0) {

		}*/
	}
}
int wmain(int argc, wchar** argv) {
	puts("Harp Script v0.1.1 (Nok Lang g0.3)");
	//For Korean
	wcin.imbue(locale("kor"));
	wcout.imbue(locale("kor"));
	COMPILE compile;
	
	bool b_outfile = true;
	if (argc == 1) {
		{
			wstring file;
			wcin >> file;
			compile.compile(file.c_str());
		}
	}
	else {
		check_argv(&compile, argc, argv, b_outfile);
	}
	
	
	exe.endcompile();
	
	if (compile.err_cnt == 0U) {
		if (b_outfile) PrintBytecode();
		clock_t start, end;
		start = clock();
		exe.state();//바이트 코드 실행
		end = clock();
		cout << "Execute time : " << (end - start) / 1000.0;
	}

	//Print ByteCode
	

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	return 0;
}