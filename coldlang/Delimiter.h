#pragma once
#include "stdafx.h"

class Delimiter : public Token {
public:
	enum DelimiterType {
		unknown,
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
		mod_assign,		// "%="

		less,			// "<"
		less_equal,		// "<="
		greater,		// ">"
		greater_equal,	// ">="
		equal,			// "=="
		not_equal,		// "!="

		_and,			// "&&"
		_or,			// "||"
		_not,			// "!"

		comma,			// ","
		colon,			// ":"
		question,		// "?"

		left_paren,		// "("
		right_paren,	// ")"

		left_bracket,	// "["
		right_bracket,	// "]"

		left_brace,		// "{"
		right_brace,	// "}"

		newline			// "\n"

	};
private:
	DelimiterType type;
public:
	// @owns: raw_string
	Delimiter(Module* _module, wchar_t * raw_string, int line_index, int column_index, DelimiterType type) :
		Token(_module, raw_string, line_index, column_index), type(type) {
	}
	wstring to_string() {
		return wstring(this->get_raw_string());
	}
	DelimiterType get_type() {
		return type;
	}
};