#include "stdafx.h"
#include "ColdLangFrontEnv.h"

using namespace std;

ColdLangFrontEnv::ColdLangFrontEnv(wstring * code) :
	lexer(new Lexer(code)),
	syntax(new Syntax(lexer)),
	code(code)
{
}


ColdLangFrontEnv::~ColdLangFrontEnv()
{
	delete lexer;
	delete syntax;
}
