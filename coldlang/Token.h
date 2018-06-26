#pragma once
#include "stdafx.h"

class Token {
private:
	wchar_t * raw_string;
	int line_index;
	int column_index;
public:
	// @owns: raw_string
	Token(wchar_t * raw_string, int line_index, int column_index) :
		raw_string(raw_string), line_index(line_index), column_index(column_index) {
		
	}
	// @lends: raw_string
	wchar_t * get_raw_string() {
		return raw_string;
	}
	int get_line_index() {
		return line_index;
	}
	int get_column_index() {
		return column_index;
	}

	virtual wstring to_string() = 0;

	~Token() {
		// @gives up: raw_string
		delete(raw_string);
	}

};