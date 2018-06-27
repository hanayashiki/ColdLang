#pragma once
#include "stdafx.h"
#include "KeywordTrie.h"

class Lexer {
private:
	int line_;
	int col_;

	const std::wstring * code_;
	std::vector<Token*> token_list;
	Module* module_;
	KeywordTrie keyword_trie_;

	int code_pointer_ = 0;
	void skip_blanks();
	wchar_t next_char();
	wchar_t peek_char();
public:
	Lexer(Module* _module) : module_(_module), code_(&module_->code) {
		line_ = 1;
		col_ = 1;
	};
	Lexer(std::wstring* code) : module_(NULL), code_(code) {};
	// @lends: Token* result
	Token * next_token();
	// @lends: Token* result
	Token * prev_token();
	Token * parse_next_token();
	Token * parse_next_word();
	~Lexer() {
		for (auto t : token_list) {
			delete(t);
		}
	}
};