#pragma once
//#define fstr(a) (#a)

enum funclib {

	f_type,		//type()
	f_types,		//types()
	f_get,		//get()
	f_getn,		//getn()
	f_gets,		//gets()
	f_getl,		//getl()
	f_getch,	//getch()
	f_int,		//int()
	f_num,		//num()
	f_str,		//str()
	f_ascii,	//ascii()
	f_char,		//char()
	f_len,		//len()
	f_size,		//size()
	f_ntime,

	/*math*/
	f_abs,		//abs()
	f_sin,		//sin()
	f_cos,		//cos()
	f_tan,		//tan()
	f_radian,		//radian()
	f_degree,		//degree()
	f_sqrt,		//sqrt()
	f_sqrtn,		//sqrtn()
	f_log,			//log(,)
	f_log2,			//log2()
	f_log10,		//log10()
	f_rand,		//rand()
	f_random,	//random(,)

	/*winbgi*/
	f_initbgi,		//bgi.initbgi()
	f_square,		//bgi.square()
	f_circle,		//bgi.circle()
	f_arc,			//bgi.arc()
	f_closebgi,		//bgi.closebgi()
	/*window*/
	f_gotoxy,		//gotoxy()
	f_color,		//color()
	f_bgcolor,		//bgcolor()
	f_setcolor,		//setcolor()
	f_drawxy,		//drawxy()
	f_beep,		//beep()
	f_title,		//title()
	f_system,	//system()
	f_cls,		//cls()
	f_sleep,	//sleep()
	f_cursor,	//cursor()
	f_hidecursor,	//hidecursor()
	f_showcursor,	//showcursor()

	/*file*/
	f_fopen,		//open()
	f_fget,		//fget()
	f_fgetn,		//fgetn()
	f_fgets,		//fgets()
	f_fgetl,		//fgetl()
	f_fout,		//fout()
	f_fgetc,
	f_fread_all,
	f_fclose,	//fclose

	/*dxlib*/
	f_dxlib_init,
	f_waitkey,
	f_msgloop,
	f_drawpixel,
	f_loadgraph,
	f_setblend,
	f_drawgraph,
	f_rotagraph,
	f_keypress,
	f_keyhit,
	f_winmode,
	f_drawstr,
	f_loadsound,
	f_playsound,
	f_drawbox,
	f_loadfont,
	f_drawfont,
	f_setarea,
	f_deletesound,
	f_stopsound,
	f_ifsound,
	f_rgb,
	f_dxlib_end,

};
//dxlib.dll에 있는 함수


using dx_winmode = void(*)(bool);
using dx_loadgraph = int(*)(const wchar_t*);
using dx_divgraph = int(*)(const wchar_t*, int, int, int, int, int, int*);
using dx_msgloop = bool(*)();
using dx_keypress = bool(*)(int i);
using dx_keyhit = bool(*)(int i);
using dx_drawgraph = void(*)(int img, float x, float y);
using dx_rotagraph = void(*)(int img, float x, float y, double angle, double scale);
using dx_setblend = void(*)(int i, int t);
using dx_init = void(*)();
using dx_end = void(*)();
using dx_color = int(*)(int, int, int);
using dx_drawstring = void(*)(const wchar_t*, int x, int y, int color);
using dx_loadsound = int(*)(const wchar_t*);
using dx_playsound = void(*)(int, int);
using dx_setvolume = void(*)(int, int);
using dx_drawbox = void(*)(int, int, int, int, int);
using dx_loadfont = int(*)(const wchar_t*, int, int);
using dx_fontstring = void(*)(const wchar_t*, int, int, int, int);
using dx_setarea = void(*)(int, int, int, int);
using dx_deletesound = void(*)(int);
using dx_stopsound = void(*)(int);
using dx_checksound = void(*)(int);
using dx_waitkey = void(*)();
using dx_drawpixel = void(*)(int, int, int);



extern HMODULE g_dll;
extern dx_init dxlib_init;
extern dx_end dxlib_end;
extern dx_waitkey waitkey;
extern dx_winmode winmode;
extern dx_loadgraph loadgraph;
extern dx_rotagraph rotagraph;
extern dx_drawpixel drawpixel;
extern dx_color getcolor;
extern dx_loadsound loadsound;
extern dx_playsound playsound;
extern dx_drawstring drawstring;
extern dx_drawgraph drawgraph;
extern dx_msgloop msgloop;
extern dx_keyhit keyhit;
extern dx_keypress keypress;
extern dx_setblend setblend;
extern dx_loadfont loadfont;
extern dx_fontstring drawfont;
extern dx_setarea setarea;
extern dx_checksound checksound;
extern CONSOLE_CURSOR_INFO cursorinfo;
extern int random(int min, int max);
extern double randnum();
extern double sqrtn(double x, double a);
extern double logx(double a, double b);
extern void gotoxy(short x, short y);
extern void setColor(int color, int bgcolor);
extern void txtColor(int color);
extern void bgColor(int bgcolor);
extern double radian(double n);
extern double degree(double n);
extern string multibyte(wstring uni);
//extern wstring get_allfile(FILE*);
extern long long file_get(FILE*);
extern double file_getn(FILE*);

