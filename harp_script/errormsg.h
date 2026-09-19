#ifndef __ERRORMSG_H__
#define __ERRORMSG_H__

#include <string>
using namespace std;
enum errortype {
	NONE=0,
	NOTYET,
	NOFILE,
	NOTOKEN,
	NOBRACK,
	NOIDENT,
	NOARRAY,
	BADSTATE,
	BADPRINT,
	BADINCLUDE,
	BADIDENT,
	OVERIDENT,
	BADFUNC,
	BADARRAY,
	FUNCSTORE,
	BADRETURN,
	NOCOMMA,
	NOMODULE,
	NOBLOCK,
	BLOCKOPEN,
	BADENUM,
	TOOMANY,
	
};
#undef ERROR
class ERRORMSG {
public:
	uint32_t err_cnt;
	ERRORMSG() { err_cnt = 0; }
	void error(const wstring &s, errortype t, uint32_t line) {
		err_cnt++;

		printf("ERROR(%d) : ", line);

		switch (t) {
		case NOFILE:     wprintf(L"Could not open file '%s'.", s.c_str()); break;
		case NOTYET:	 wprintf(L"The feature is unimplemented."); break;
		case NOTOKEN:    wprintf(L"Unknown identifier or token '%s'", s.c_str()); break;
		case NOBRACK:    wprintf(L"Please close the parentheses."); break;
		case NOIDENT:    wprintf(L"Please declare the identifier '%s'.", s.c_str()); break;
		case NOARRAY:    wprintf(L"Please close the curly braces."); break;
		case BADSTATE:   wprintf(L"Invalid statement."); break;
		case BADPRINT:   wprintf(L"Please enter the print statement correctly."); break;
		case BADINCLUDE: wprintf(L"Please enter a valid file name."); break;
		case BADIDENT:   wprintf(L"'%s' is an invalid identifier name.", s.c_str()); break;
		case BADARRAY:   wprintf(L"Invalid array declaration."); break;
		case OVERIDENT:  wprintf(L"Identifier '%s' is already declared.", s.c_str()); break;
		case BADFUNC:    wprintf(L"Please declare the function correctly."); break;
		case FUNCSTORE:  wprintf(L"Functions cannot be assigned."); break;
		case BADRETURN:  wprintf(L"Return can only be used within a function."); break;
		case NOCOMMA:    wprintf(L"',' is required."); break;
		case NOMODULE:   wprintf(L"'%s' is a non-existent module.", s.c_str()); break;
		case NOBLOCK:    wprintf(L"A block is required."); break;
		case BLOCKOPEN:  wprintf(L"Please close the block."); break;
		case BADENUM:    wprintf(L"Only numbers and strings can be used as enumeration constants."); break;
		case TOOMANY:    wprintf(L"Too many statements."); break;
		}
		puts("");
		/*
		case NOFILE:buf += L"파일 '" + s + L"'을(를) 열 수 없습니다."; break;
		case NOTOKEN:buf += L"알 수 없는 식별자 혹은 토큰 '" + s + L"'"; break;
		case NOBRACK:buf += L"괄호를 닫아주세요."; break;
		case NOIDENT:buf += L"식별자 '" + s + L"'를 선언해 주세요."; break;
		case NOARRAY:buf += L"중괄호를 닫아 주세요."; break;
		case BADSTATE:buf += L"올바르지 않은 문입니다."; break;
		case BADPRINT:buf += L"print문을 제대로 입력해 주십시오."; break;
		case BADINCLUDE:buf += L"제대로 된 파일 이름을 입력해 주십시오."; break;
		case BADIDENT:buf += L"'" + s + L"'는 올바르지 않은 식별자명 입니다."; break;
		case BADARRAY:buf += L"배열 선언이 잘못되었습니다."; break;
		case OVERIDENT:buf += L"식별자 '"+s+L"'가 이미 선언되어 있습니다."; break;
		case BADFUNC:buf += L"함수를 제대로 선언해 주십시오."; break;
		case FUNCSTORE:buf += L"함수는 대입할 수 없습니다."; break;
		case BADRETURN:buf += L"리턴은 함수 내에서만 사용 가능합니다."; break;
		case NOCOMMA:buf += L",가 필요합니다."; break;
		case NOMODULE:buf += L"'" + s + L"'은 존재하지 않는 모듈입니다.";
		case NOBLOCK:buf += L"블록이 필요합니다.";
		case BLOCKOPEN:buf += L"블록을 닫아주세요.";
		case BADENUM:buf += L"열거형 상수로는 숫자, 문자열만 사용 가능합니다.";
		*/

	}
};

#endif