#pragma once
#include "stdafx.h"

class Lexer;

class DelimiterParseState {
private:
	DelimiterParseState* ascii_to_state_[128];
	Delimiter::DelimiterType result_tag_;
public:
	DelimiterParseState() {
		memset(ascii_to_state_, 0, sizeof(ascii_to_state_));
		result_tag_ = Delimiter::unknown;
	}
	DelimiterParseState(Delimiter::DelimiterType result_tag) : result_tag_(result_tag) {
		memset(ascii_to_state_, 0, sizeof(ascii_to_state_));
	}
	DelimiterParseState* add_state(char c, DelimiterParseState* s) {
		this->ascii_to_state_[c] = s;
		return this;
	}
	DelimiterParseState* get_next_state(wchar_t c, bool & accept_flag, bool & bad, bool & move) {
		accept_flag = !(result_tag_ == Delimiter::unknown);
		if (accept_flag) {
			bad = false;
			move = false;
			return NULL;
		}
		if ((c < 0 || c > 127 || ascii_to_state_[c] == NULL)) {
			bad = true;
		}
		else {
			bad = false;
		}
		if (bad && ascii_to_state_[0] != NULL) {
			move = false;
		}
		else {
			move = true;
		}
		if (bad == false) {
			return ascii_to_state_[c];
		}
		else {
			return NULL;
		}
	}
	Delimiter::DelimiterType get_result_tag_() {
		return this->result_tag_;
	}
	~DelimiterParseState() {
		for (int i = 0; i < sizeof(ascii_to_state_) / sizeof(ascii_to_state_[0]); i++) {
			if (ascii_to_state_[i] != NULL) {
				delete ascii_to_state_[i];
			}
		}
	}
};

class DelimiterParser
{
private:
	DelimiterParseState * root;
	Lexer * lexer_;
public:
	DelimiterParser(Lexer* lexer);
	Delimiter * parse();
	~DelimiterParser() {
		delete root;
	}

};