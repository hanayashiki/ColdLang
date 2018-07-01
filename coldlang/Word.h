#pragma once
#include "stdafx.h"
#include "Module.h"
#include "Token.h"

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
		keyword_return,
		keyword_task
	};
private:
	WordType type;
public:
	// @owns: raw_string
	Word(Module* _module, wchar_t * raw_string, int line_index, int column_index, WordType type) :
		Token(_module, raw_string, line_index, column_index), type(type) {
	}
	std::wstring to_string() {
		return std::wstring(this->get_raw_string());
	}
	wstring to_xml()
	{
		if (type == identifier) {
			return tutils::to_xml_quoted(L"word", to_string());
		} else
		{
			return tutils::to_xml_quoted(L"keyword", to_string());
		}
	}
	wchar_t* get_word() {
		return this->get_raw_string();
	}
	WordType get_type() {
		return type;
	}

};