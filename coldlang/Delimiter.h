#pragma once
#include "stdafx.h"

class Delimiter : public Token {
public:
	enum DelimiterType {
		increment,		// "++"
		decrement,		// "--"

		add,			// "+"
		minus,			// "-"
		star,			// "*"
		divide,			// "/"
		mod,			// "%"

		assign,			// "="
		add_assign,		// "+="
		sub_assign,		// "-="
		mult_assign,	// "*="
		div_assign,		// "/="

		less,			// "<"
		less_equal,		// "<="
		greater,		// ">"
		greater_equal,	// ">="
		equal,			// "=="
		not_equal,		// "!="

		_and,			// "&&"
		_or,			// "||"

		single_quote,	// "\'"
		double_quote,	// "\""

		comma			// ","
	};
private:
	DelimiterType type;
public:
	// @owns: raw_string
	Delimiter(wchar_t * raw_string, int line_index, int column_index, DelimiterType type) :
		Token(raw_string, line_index, column_index), type(type) {
	}
	wstring to_string() {
		return wstring(this->get_raw_string);
	}
	DelimiterType get_type() {
		return type;
	}
};