/*
Toy script by Noksek2

lol

*/
#include "compile.h"
#include <time.h>

RandomDevice* g_randdev;
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
	//opp[op_not2] = "NOT2";
	opp[op_eq] = "EQ";
	opp[op_gte] = "GTE";
	opp[op_gt] = "GT";
	opp[op_lte] = "LTE";
	opp[op_lt] = "LT";
	opp[op_add] = "ADD";
	opp[op_sub] = "SUB";
	opp[op_mul] = "MUL";
	opp[op_div] = "DIV";
	opp[op_mod] = "MOD";
	opp[op_pow] = "POW";
	opp[op_min] = "MIN";
	opp[op_not] = "NOT";
	opp[op_print] = "PRINT";
	opp[op_out] = "OUT";

	opp[op_addeq] = "ADDEQ";
	opp[op_subeq] = "SUBEQ";
	opp[op_muleq] = "MULEQ";
	opp[op_diveq] = "DIVEQ";
	opp[op_modeq] = "MODEQ";
	opp[op_poweq] = "POWEQ";
	opp[op_ang] = "ANG";
	ofstream out("bytecode.txt");
	out << "========symtable========\n";
	out << "no	type	mem	func	frame\n";
	char buf[][7] = {"Var","Func","InFunc"};
	int buf_no;
	for (uint32_t i = 0; i < exe.symtable.size(); i++) {
		switch (exe.symtable[i].t) {
		case SVar: buf_no = 0; break;
		case SFunc:buf_no = 1; break;
		default: buf_no = 2; break;
		}
		out << i << '\t' << buf[buf_no] << '\t' << exe.symtable[i].mem << '\t' 
			<< exe.symtable[i].func << "\t" << exe.symtable[i].frame << "\n";
	}
	out << "\n\n\n========bytecode========\n";
	for (uint32_t i = 0; i < bytecode.m_len; i++) {
		out << opp[bytecode[i].op] << " " << bytecode[i].opr << "\n";
	}
}

enum RunFlag {
	RunFlag_Dis,//printbytecode
	RunFlag_Debug,
	RunFlag_Eng,
	MAX_FLAG = 128,
};
bool g_runflags[MAX_FLAG];

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
void HarpContext_Init() {
	g_randdev = new RandomDevice();
}
void HarpContext_Delete(){
	if (g_randdev) {
		delete g_randdev;
		g_randdev = NULL;
	}
}
void HaroContext_SetFlagDefault() {
	g_runflags[RunFlag_Dis] = true;
	g_runflags[RunFlag_Eng] = true;
}
int wmain(int argc, wchar** argv) {
	HarpContext_Init();
	HarpContext_Delete();

	puts("Harp Script v0.2.0 (Nok Lang g0.3)");
	//For Korean
	wcin.imbue(locale("kor"));
	wcout.imbue(locale("kor"));
	COMPILE compile;
	
	
	if (argc == 1) {
		{
			wcout << ">>";
			wstring file(256, 0);
			wcin.getline(&file[0], 256);
			
			compile.compile(file.c_str());
		}
	}
	else {
		check_argv(&compile, argc, argv, g_runflags[RunFlag_Dis]);
	}
	
	
	exe.endcompile();
	
	if (ERRORMSG::err_cnt == 0U) {
		if (g_runflags[RunFlag_Dis]) PrintBytecode();
		clock_t start, end;
		start = clock();
		exe.run();//바이트 코드 실행
		end = clock();
		cout << "Execute time : " << (end - start) / 1000.0;
	}

	//Print ByteCode
	

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	return 0;
}