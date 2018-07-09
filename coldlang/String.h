#pragma once
#include "stdafx.h"

class Token;

class String : public Token {
private:
	wchar_t* value;
public:
	enum StringType {
		GeneralString
	};
	// @owns: raw_string, value
	String(Module * module, wchar_t * raw_string, int line_index, int column_index, wchar_t * value) :
		Token(module, raw_string, line_index, column_index), value(value) {
	}
	wstring to_string() {
		return wstring(this->get_raw_string());
	}
	wchar_t* get_value() {
		return this->value;
	}
	StringType get_type()
	{
		// TODO: this is temporary
		return GeneralString;
	}
	wstring to_xml()
	{
		return tutils::to_xml_quoted(L"string", to_string());
	}
	~String() {
		delete value;
	}
};
