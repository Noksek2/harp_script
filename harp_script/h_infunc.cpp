//#load_dll dxlib.dll
//func dxlib_end()
//func dxlib_init()
//func dxlib_loadgraph()
//func dxlib_drawgraph(int,float,)

// arr < 3

/*런타임 시 사용하는 C 함수들*/
#include "h_exec.h"

HMODULE g_dll;
dx_init dxlib_init;
dx_end dxlib_end;
dx_waitkey waitkey;
dx_winmode winmode;
dx_loadgraph loadgraph;
dx_rotagraph rotagraph;
dx_drawpixel drawpixel;
dx_color getcolor;
dx_loadsound loadsound;
dx_playsound playsound;
dx_drawstring drawstring;
dx_drawgraph drawgraph;
dx_msgloop msgloop;
dx_keyhit keyhit;
dx_keypress keypress;
dx_setblend setblend;
dx_loadfont loadfont;
dx_fontstring drawfont;
dx_setarea setarea;
dx_checksound checksound;

CONSOLE_CURSOR_INFO cursorinfo;




int random(int64_t min, int64_t max) {
	return (int)(((double)rand() / RAND_MAX) * (max - min)) + min;
}
double randnum() {
	return (double)rand() / RAND_MAX;
}
double sqrtn(double x, double a) {
	return pow(a, 1 / x);
}
double logx(double a, double b) {
	return log(b) / log(a);
}

//wstring get_allfile(FILE* f) {
//	fseek(f, 0, SEEK_END);
//	UINT size = ftell(f);
//	string s;
//	s.resize(size + 1);
//	fseek(f, 0, SEEK_SET);
//	fread(&s[0], size, 1, f);
//	s[size] = 0;
//	fclose(f);
//
//	UINT us = MultiByteToWideChar(CP_ACP, 0, &s[0], size, 0, 0);
//	wstring ubuf;
//	ubuf.resize(us + 1);
//	MultiByteToWideChar(CP_ACP, 0, &s[0], size, &ubuf[0], us);
//	ubuf[us] = 0;
//	return ubuf;
//}
long long file_get(FILE* f) {
	long long a = 0;
	fscanf_s(f, "%lld", &a);
	return a;
}
double file_getn(FILE* f) {
	double d = 0.0;
	fscanf(f, "%lf", &d);
	return d;
}

string multibyte(wstring uni) {
	int len = WideCharToMultiByte(CP_ACP, 0, &uni[0], -1, NULL, 0, NULL, NULL);
	string strMulti(len, 0);
	WideCharToMultiByte(CP_ACP, 0, &uni[0], -1, &strMulti[0], len, NULL, NULL);
	return strMulti;
}
void gotoxy(short x, short y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void setColor(int color, int bgcolor) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((bgcolor & 0xf) << 4) | (color & 0xf));
}
void txtColor(int color) {
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (info.wAttributes & 0xf0) | (color & 0xf));
}
void bgColor(int bgcolor) {
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((bgcolor & 0xf) << 4) | (info.wAttributes & 0xf));
}
double radian(double n) {
	return n * MATH_PI / 180;
}
double degree(double n) {
	return n / MATH_PI * 180;
}

static harpobj make_typestr_obj(harpdata dat) {
   harpstr str;
   uint32_t type;
   switch (dat.GetType()) {
   case TInt:
       str = harpstr_new(L"int", 3); break;
   case TUnq:
       str = harpstr_new(L"uniq", 4); break;
   case TError:
       str = harpstr_new(L"err", 3); break;
   case TObj:
       str = harpstr_new(L"obj", 3); break;
	   //dat.decRC();
   //case TStr:
   //    str = harpstr_new(L"instr", 5); break;
   case TFnc:
       str = harpstr_new(L"fnc", 4); break;
   case TFloat:
       str = harpstr_new(L"float", 5); break;
   default:
	   str = harpstr_new(L"?", 1); break;
   }
   return harpobj_new(str, objt_str);

}
static harpdata make_typestr(harpdata n0) {

	harpobj* obj = g_objpool->Insert(make_typestr_obj(n0));

	n0.DecRC();
	n0.SetObj(obj);
}
inline const harpdata harp_fclose(harpdata n0) {
	harpdata h;
	if (!IF_INT(n0)) {
		DECODE_OBJ(n0)->DecRC();
		h.byte = ENCODE_ERR(rte_func_para_no_match);
		return h;
	}
	FILE* fp = (FILE*)DECODE_INT(n0);
	if (fclose(fp) < 0) {
		h.byte = ENCODE_ERR(rte_unknown);
		
	}
	return h;
}
inline const harpdata harp_fopen(harpdata n0, harpdata n1) {
	harpdata d;
	FILE * fp;
	if (!IF_OBJ(n0)) goto l_err;
	if (!IF_OBJ(n1)) goto l_err;
	
	if (DECODE_OBJ(n0)->objtype != objt_str) goto l_err;
	if (DECODE_OBJ(n1)->objtype != objt_str) goto l_err;

	_wfopen_s(&fp, n0.GetStr()->ptr, n1.GetStr()->ptr);
	
	d.SetInt((uint64_t)fp);
	goto l_dec;
l_err:
	d.byte = ENCODE_ERR(rte_func_para_no_match);
l_dec: 
	DECODE_OBJ(n0)->DecRC();
	DECODE_OBJ(n1)->DecRC();
	
	return d;
}
inline const harpdata harp_rand() {
	harpdata d;
	d.f64 = g_randdev->GetRand_f();
	return d;
}
inline const harpdata harp_rand(harpdata n0) {
	harpdata d;
	if (!IF_INT(n0)) goto l_err;
	d.byte = DECODE_INT_V(g_randdev->GetRand_i(DECODE_INT(n0)));
	return d;
l_err:
	{
		d.byte = ENCODE_ERR(rte_func_para_no_match);
		n0.DecRC();
		return d;
	}
}
inline const harpdata harp_rand(harpdata n0, harpdata n1) {
	harpdata d;
	if (!IF_INT(n0)) goto l_err;
	if (!IF_INT(n1)) goto l_err;
	//
	{
		d.byte = DECODE_INT_V(g_randdev->GetRand_i(DECODE_INT(n0), DECODE_INT(n1)));
		return d;
	}
l_err: {
	d.byte = ENCODE_ERR(rte_func_para_no_match);
	n0.DecRC();
	n1.DecRC();
	return d;
	}
}




void infunc_call(infunctype ft, harpdata* dats, uint32_t para_len) {
//#define ASSERT_PARA(N, BUF) Harp_assert(para_len == (N), BUF);
//#define ASSERT_PARA_DEF(N) ASSERT_PARA(N, "[ERROR] count of para unmatched");
#define ASSERT_PARA_DEF(PARA_CNT) if(para_len != PARA_CNT)N[0].SetErr(rte_func_para_no_match);
	harpdata* const N = dats - para_len;
	switch (ft) {
	case f_type:case f_types: {
		ASSERT_PARA_DEF(1);
		N[0] = make_typestr(N[0]);
		
	}break;
	case f_fopen: {
		ASSERT_PARA_DEF(2);
		N[0] = harp_fopen(N[0], N[1]);
		break;
	}
	case f_fclose:
		break;
	case f_gets:
		for (uint32_t i = 0; i < para_len; i++)
			N[i].Print();
		break;
	case f_abs: 
		ASSERT_PARA_DEF(1);
		if (IF_INT(N[0])) {
			if (N[0].byte & 0x800000000000llu) {
				N[0].byte ^= 0x800000000000llu;
			}
		}
		else if (IF_FLOAT(N[0])) {
			if (N[0].f64 < 0) N[0].f64 *= -1.0;
		}
		else {
			N[0].SetErr(rte_unknown);
		}
		break;
	case f_sin:
	case f_cos:
	case f_rand:N[0] = harp_rand(); break;
	case f_random:
		if(para_len == 0){ N[0] = harp_rand(); }
		if (para_len == 1) {
			N[0] = harp_rand(N[0]);
		}
		else if (para_len == 2) {
			N[0] = harp_rand(N[0], N[1]);
		}
		else N[0].SetErr(rte_func_para_no_match);
		break;
	default:
		N[0].SetErr(rte_infunc_unknown);
		break;
	}
}

void EXECUTE::callinfunc(infunctype t) {
	//mstack[0];
	//infunc_call(t,mstack.back_ptr(),_);
	//switch (infunc_type) {
	//	// 기본 타입 및 입력 함수 
	//case f_type: opstack.push(make_typestr(mstack[0])); return;
		/*case f_ascii: opstack.push((int)mstack[0].s[0]); return;
		case f_get: return;
		case f_getn:return;
		case f_gets:return;
		case f_ntime: opstack.push((int)clock()); return;
		case f_len: opstack.push((int)mstack[0].s.size()); return;
		case f_int: opstack.push(IF_FLOAT(mstack[0]) ? (int)DECODE_FLOAT(mstack[0]) : (mstack[0].type == Str ? stoll(mstack[0].s) : 0)); return;
		case f_num: opstack.push(IF_INT(mstack[0]) ? (double)DECODE_INT(mstack[0]) : (mstack[0].type == Str ? stod(mstack[0].s) : 0)); return;
		case f_str: opstack.push(IF_INT(mstack[0]) ? to_wstring(DECODE_INT(mstack[0])) : (IF_FLOAT(mstack[0]) ? to_wstring(DECODE_FLOAT(mstack[0])) : L"")); return;
		case f_char: opstack.push((wchar_t)DECODE_INT(mstack[0])); return;

		   // math 함수
		case f_abs: opstack.push(IF_INT(mstack[0]) ? abs(DECODE_INT(mstack[0])) : abs(DECODE_FLOAT(mstack[0]))); return;
		case f_sin: opstack.push(IF_INT(mstack[0]) ? sin(DECODE_INT(mstack[0]) * MATH_PI / 180) : sin(DECODE_FLOAT(mstack[0]) * MATH_PI / 180)); return;
		case f_cos: opstack.push(IF_INT(mstack[0]) ? cos(DECODE_INT(mstack[0]) * MATH_PI / 180) : cos(DECODE_FLOAT(mstack[0]) * MATH_PI / 180)); return;
		case f_degree: opstack.push(IF_INT(mstack[0]) ? degree((double)DECODE_INT(mstack[0])) : degree(DECODE_FLOAT(mstack[0]))); return;
		case f_radian: opstack.push(IF_INT(mstack[0]) ? radian((double)DECODE_INT(mstack[0])) : radian(DECODE_FLOAT(mstack[0]))); return;
		case f_rand: opstack.push(randnum()); return;
		case f_random: opstack.push(random((IF_INT(mstack[1]) ? DECODE_INT(mstack[1]) : (int)DECODE_FLOAT(mstack[1])), (IF_INT(mstack[0]) ? DECODE_INT(mstack[0]) : (int)DECODE_FLOAT(mstack[0])))); return;

			// window 함수
		case f_gotoxy: gotoxy((short)DECODE_INT(mstack[1]), (short)DECODE_INT(mstack[0])); break;
		case f_setcolor: setColor(DECODE_INT(mstack[1]), DECODE_INT(mstack[0])); break;
		case f_color: txtColor(DECODE_INT(mstack[0])); break;
		case f_bgcolor: bgColor(DECODE_INT(mstack[0])); break;
		case f_cls: system("cls"); break;
		case f_system: system(multibyte(mstack[0].s).c_str()); break;
		case f_beep: Beep(DECODE_INT(mstack[1]), DECODE_INT(mstack[0])); break;
		case f_title: SetConsoleTitle(mstack[0].s.c_str()); break;
		case f_sleep: Sleep((DWORD)DECODE_INT(mstack[0])); break;
		case f_cursor:
			cursorinfo.bVisible = 1;
			cursorinfo.dwSize = (DWORD)(IF_INT(mstack[0]) ? DECODE_INT(mstack[0]) : DECODE_FLOAT(mstack[0]));
			SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorinfo);
			break;
		case f_showcursor:
			cursorinfo.bVisible = 1;
			SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorinfo);
			break;
		case f_hidecursor:
			cursorinfo.bVisible = 0;
			SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorinfo);
			break;

			// file 함수
		case f_fopen: opstack.push((int64_t)_wfopen(mstack[1].s.c_str(), mstack[0].s.c_str())); return;
		case f_fclose: fclose((FILE*)DECODE_INT(mstack[0])); break;
		case f_fgetc: opstack.push((wchar_t)fgetc((FILE*)DECODE_INT(mstack[0]))); return;
		case f_fget: opstack.push(file_get((FILE*)DECODE_INT(mstack[0]))); return;
		case f_fgetn: opstack.push(file_getn((FILE*)DECODE_INT(mstack[0]))); return;
		case f_fread_all: opstack.push(getallfile((FILE*)DECODE_INT(mstack[0]))); return;

			// dxlib 함수
		case f_dxlib_init: dxlib_init(); break;
		case f_msgloop: opstack.push(msgloop()); return;
		case f_waitkey: waitkey(); break;
		case f_winmode: winmode(DECODE_INT(mstack[0])); break;
		case f_loadgraph: opstack.push(loadgraph(mstack[0].s.c_str())); return;
		case f_drawgraph: drawgraph(DECODE_INT(mstack[2]), (IF_INT(mstack[1]) ? DECODE_INT(mstack[1]) : (float)DECODE_FLOAT(mstack[1])), (IF_INT(mstack[0]) ? DECODE_INT(mstack[0]) : (float)DECODE_FLOAT(mstack[0]))); break;
		case f_rgb: opstack.push(getcolor(DECODE_INT(mstack[2]), DECODE_INT(mstack[1]), DECODE_INT(mstack[0]))); return;
		case f_drawpixel: drawpixel(DECODE_INT(mstack[2]), DECODE_INT(mstack[1]), DECODE_INT(mstack[0])); break;
		case f_drawstr: drawstring(mstack[3].s.c_str(), (IF_INT(mstack[2]) ? DECODE_INT(mstack[2]) : (int)DECODE_FLOAT(mstack[2])), (IF_INT(mstack[1]) ? DECODE_INT(mstack[1]) : (int)DECODE_FLOAT(mstack[1])), (IF_INT(mstack[0]) ? DECODE_INT(mstack[0]) : (int)DECODE_FLOAT(mstack[0]))); break;
		case f_rotagraph:
			rotagraph(DECODE_INT(mstack[4]),
					  (IF_INT(mstack[3]) ? DECODE_INT(mstack[3]) : (float)DECODE_FLOAT(mstack[3])),
					  (IF_INT(mstack[2]) ? DECODE_INT(mstack[2]) : (float)DECODE_FLOAT(mstack[2])),
					  (IF_INT(mstack[1]) ? DECODE_INT(mstack[1]) : DECODE_FLOAT(mstack[1])) * MATH_PI / 180.0,
					  (IF_INT(mstack[0]) ? DECODE_INT(mstack[0]) : DECODE_FLOAT(mstack[0])));
			break;
		case f_keypress: opstack.push(keypress(DECODE_INT(mstack[0]))); return;
		case f_keyhit: opstack.push(keyhit(DECODE_INT(mstack[0]))); return;
		case f_dxlib_end: dxlib_end(); break;
		}
		opstack.push(1);*/
		//opstack.push(harpdata{ 1 });
	//}
}
	bool EXECUTE::usefunc(const wstring & s) {

	if (s == L"basic") {//basic은 기본적으로 include 됨.
		pushinfunc(L"type", f_type);
		pushinfunc(L"len", f_len);
		pushinfunc(L"ascii", f_ascii);
		//pushinfunc(L"char", f_char);
		pushinfunc(L"int", f_int);
		pushinfunc(L"num", f_num);
		pushinfunc(L"str", f_str);
		pushinfunc(L"get", f_get);
		pushinfunc(L"getn", f_getn);
		pushinfunc(L"gets", f_gets);
		pushinfunc(L"ntime", f_ntime);
		pushinfunc(L"size", f_size);
		return 1;
	}
	else if (s == L"math") {//math 기능
		pushenum(L"pi", MATH_PI);
		pushinfunc(L"abs", f_abs);
		pushinfunc(L"sin", f_sin);
		pushinfunc(L"cos", f_cos);
		pushinfunc(L"rand", f_rand);
		pushinfunc(L"random", f_random);
		pushinfunc(L"degree", f_degree);
		pushinfunc(L"radian", f_radian);

		return 1;
	}
	else if (s == L"window") {//window 기능
		//상수
		pushenum(L"cmd_black", 0);
		pushenum(L"cmd_blue", 1);
		pushenum(L"cmd_green", 2);
		pushenum(L"cmd_aqua", 3);
		pushenum(L"cmd_red", 4);
		pushenum(L"cmd_purple", 5);
		pushenum(L"cmd_yellow", 6);
		pushenum(L"cmd_lgray", 7);
		pushenum(L"cmd_gray", 8);
		pushenum(L"cmd_lblue", 9);
		pushenum(L"cmd_lgreen", 10);
		pushenum(L"cmd_sky", 11);
		pushenum(L"cmd_lred", 12);
		pushenum(L"cmd_lpurple", 13);
		pushenum(L"cmd_skin", 14);
		pushenum(L"cmd_white", 15);
		pushenum(L"mb_ok", MB_OK);
		pushenum(L"mb_error", MB_ICONERROR);
		pushenum(L"mb_help", MB_HELP);//
		pushenum(L"mb_okno", MB_OKCANCEL);//
		pushenum(L"mb_yesno", MB_YESNO);//
		pushenum(L"mb_hand", MB_ICONHAND);

		//함수
		pushinfunc(L"beep", f_beep);
		pushinfunc(L"gotoxy", f_gotoxy);
		pushinfunc(L"setcolor", f_setcolor);
		pushinfunc(L"color", f_color);
		pushinfunc(L"bgcolor", f_bgcolor);
		pushinfunc(L"title", f_title);
		pushinfunc(L"cls", f_cls);
		pushinfunc(L"system", f_system);
		pushinfunc(L"sleep", f_sleep);
		pushinfunc(L"cursor", f_cursor);
		pushinfunc(L"hidecursor", f_hidecursor);
		pushinfunc(L"showcursor", f_showcursor);
		return 1;
	}
	else if (s == L"file") {//왜 안 만들었노 시바련아
		pushinfunc(L"fopen", f_fopen);
		pushinfunc(L"fclose", f_fclose);
		pushinfunc(L"fgetc", f_fgetc);
		pushinfunc(L"fget", f_fget);
		pushinfunc(L"fgetn", f_fgetn);
		pushinfunc(L"fread_all", f_fread_all);
		return 1;
	}
	else if (s == L"winbgi") {//이것도 왜 안 만들었노 시바견아
		return 1;
	}
	else if (s == L"dxlib") {//dxlib. 근데 만들다가 그만듬.
		//키 상수
		pushenum(L"vk_left", VK_LEFT);
		pushenum(L"vk_right", VK_RIGHT);
		pushenum(L"vk_up", VK_UP);
		pushenum(L"vk_down", VK_DOWN);
		pushenum(L"vk_shift", VK_SHIFT);
		pushenum(L"vk_esc", VK_ESCAPE);
		pushenum(L"vk_space", VK_SPACE);
		pushenum(L"vk_ctrl", VK_CONTROL);
		pushenum(L"vk_enter", VK_RETURN);
		//왜 이렇게 적어

		//dxlib 함수
		pushinfunc(L"dxlib_init", f_dxlib_init);
		pushinfunc(L"waitkey", f_waitkey);
		pushinfunc(L"msgloop", f_msgloop);
		pushinfunc(L"loadgraph", f_loadgraph);
		pushinfunc(L"drawgraph", f_drawgraph);
		pushinfunc(L"rotagraph", f_rotagraph);
		pushinfunc(L"keypress", f_keypress);
		pushinfunc(L"keyhit", f_keyhit);
		pushinfunc(L"drawpixel", f_drawpixel);
		pushinfunc(L"drawstr", f_drawstr);
		pushinfunc(L"winmode", f_winmode);
		pushinfunc(L"rgb", f_rgb);
		pushinfunc(L"rgb", f_rgb);
		pushinfunc(L"dxlib_end", f_dxlib_end);
		if (!g_dll) {//dll 불러오는 곳
			g_dll = LoadLibrary(L"DxLib.g_dll");
			dxlib_init = (dx_init)GetProcAddress(g_dll, "dx_init");
			dxlib_end = (dx_end)GetProcAddress(g_dll, "dx_end");
			waitkey = (dx_waitkey)GetProcAddress(g_dll, "dx_waitkey");
			winmode = (dx_winmode)GetProcAddress(g_dll, "dx_winmode");
			loadgraph = (dx_loadgraph)GetProcAddress(g_dll, "dx_loadgraph");
			rotagraph = (dx_rotagraph)GetProcAddress(g_dll, "dx_rotagraph");
			drawpixel = (dx_drawpixel)GetProcAddress(g_dll, "dx_drawpixel");
			getcolor = (dx_color)GetProcAddress(g_dll, "dx_color");
			loadsound = (dx_loadsound)GetProcAddress(g_dll, "dx_loadsound");
			playsound = (dx_playsound)GetProcAddress(g_dll, "dx_playsound");
			drawstring = (dx_drawstring)GetProcAddress(g_dll, "dx_drawstring");
			drawgraph = (dx_drawgraph)GetProcAddress(g_dll, "dx_drawgraph");
			msgloop = (dx_msgloop)GetProcAddress(g_dll, "dx_msgloop");
			keyhit = (dx_keyhit)GetProcAddress(g_dll, "dx_keyhit");
			keypress = (dx_keypress)GetProcAddress(g_dll, "dx_keypress");
			setblend = (dx_setblend)GetProcAddress(g_dll, "dx_setblend");
			loadfont = (dx_loadfont)GetProcAddress(g_dll, "dx_loadfont");
			drawfont = (dx_fontstring)GetProcAddress(g_dll, "dx_fontstring");
			setarea = (dx_setarea)GetProcAddress(g_dll, "dx_setarea");
			loadsound = (dx_loadsound)GetProcAddress(g_dll, "dx_loadsound");
			playsound = (dx_playsound)GetProcAddress(g_dll, "dx_playsound");
			checksound = (dx_checksound)GetProcAddress(g_dll, "dx_checksound");
		}
		return 1;
	}
	return false;
};