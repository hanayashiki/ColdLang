#pragma once
class ColdLangFrontEnv
{
public:
	Lexer* lexer;
	Syntax* syntax;
	wstring* code;
	ColdLangFrontEnv(wstring* code);
	~ColdLangFrontEnv();
};

