#pragma once
#include "stdafx.h"
#include "Module.h"
#include "Tutils.h"

// @owns: raw_string
class Token {
private:
	wchar_t * raw_string_;
	int line_index_;
	int column_index_;
	Module * module_;

public:
	// @owns: raw_string
	Token(Module* module, wchar_t * raw_string, int line_index, int column_index) :
		module_(module), raw_string_(raw_string), line_index_(line_index_), column_index_(column_index_) {
		
	}
	// @lends: raw_string
	wchar_t * get_raw_string() const {
		return raw_string_;
	}
	int get_line_index() const {
		return line_index_;
	}
	int get_column_index() const {
		return column_index_;
	}

	virtual std::wstring to_string() {
		return wstring(raw_string_);
	}

	virtual std::wstring to_xml()
	{
		return tutils::to_xml_single_tag(L"token");
	}

	// @gives up: raw_string
	virtual ~Token() {
		delete raw_string_;
	}

};
