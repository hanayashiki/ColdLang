#pragma once
#include "stdafx.h"

class Float : public Token {
private:
	double value;
public:
	// @owns: raw_string
	Float(IR::Module * module, wchar_t * raw_string, int line_index, int column_index, double value) :
		Token(module, raw_string, line_index, column_index), value(value) {
	}
	wstring to_string() {
		return wstring(this->get_raw_string());
	}
	double get_value() {
		return this->value;
	}
	wstring to_xml()
	{
		return tutils::to_xml_quoted(L"float", to_string());
	}
};