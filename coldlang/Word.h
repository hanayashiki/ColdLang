#pragma once
#include "stdafx.h"

class Word : public Token {
public:
	enum WordType {
		identifier,
		keyword_fn,
		keyword_class,
		keyword_struct,
		keyword_use,
		keyword_if,
		keyword_while,
		keyword_for,
		keyword_return
	};
private:
	WordType type;
public:
	// @owns: raw_string
	Word(wchar_t * raw_string, int line_index, int column_index, WordType type) :
		Token(raw_string, line_index, column_index), type(type) {
	}
	wstring to_string() {
		return wstring(this->get_raw_string());
	}
	wchar_t* get_word() {
		return this->get_raw_string();
	}
	WordType get_type() {
		return type;
	}
};