#pragma once
#include "stdafx.h"

class Lexer {
private:
	int line_;
	int col_;

	const std::wstring * code_;
	std::vector<Token*> token_list;
	Module* module_;

	int code_pointer_ = 0;
	void skip_blanks();
	wchar_t nextChar();
	wchar_t peekChar();
	Token * parseNextToken();
	Token * parseNextWord();
public:
	Lexer(Module* _module) : module_(_module), code_(&module_->code) {
		line_ = 1;
		col_ = 1;
	};
	Lexer(std::wstring* code) : module_(NULL), code_(code) {};
	// @lends: Token* result
	Token * nextToken();
	// @lends: Token* result
	Token * prevToken();
	~Lexer() {
		for (auto t : token_list) {
			delete(t);
		}
	}
};