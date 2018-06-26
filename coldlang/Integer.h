#pragma once
#include "stdafx.h"

class Integer : public Token {
private:
	int64_t value;
public:
	// @owns: raw_string
	Integer(wchar_t * raw_string, int line_index, int column_index, int64_t value) :
		Token(raw_string, line_index, column_index), value(value) {
	}
	wstring to_string() {
		return wstring(this->get_raw_string());
	}
	int64_t get_value() {
		return this->value;
	}
};