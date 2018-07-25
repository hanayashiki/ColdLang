#pragma once
#include "stdafx.h"

class Integer : public Token {
private:
	uint64_t value;
public:
	// @owns: raw_string
	Integer(IR::Module* module, wchar_t * raw_string, int line_index, int column_index, uint64_t value) :
		Token(module, raw_string, line_index, column_index), value(value) {
	}
	wstring to_string() {
		return wstring(this->get_raw_string());
	}
	uint64_t get_value() {
		return this->value;
	}
	wstring to_xml()
	{
		return tutils::to_xml_quoted(L"int", to_string());
	}
};