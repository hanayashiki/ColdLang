#pragma once
#include "stdafx.h"

class Lexer {
private:
	const std::wstring * code;
	std::vector<Token*> tokenList;

	int code_pointer = 0;
	void skip_blanks();
	wchar_t nextChar();
	wchar_t peekChar();
	Token * parseNextToken();
public:
	Lexer(const std::wstring * code) : code(code) {};
	// @lends: Token* result
	Token * nextToken();
	// @lends: Token* result
	Token * prevToken();
	~Lexer() {
		for (auto t : tokenList) {
			delete(t);
		}
	}
};