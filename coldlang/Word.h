#pragma once
#include "stdafx.h"
#include "Module.h"
#include "Token.h"
#include <memory>

class Word : public Token {
public:
	enum WordType {
		identifier,
		keyword_fn,
		keyword_class,
		keyword_struct,
		keyword_use,
		keyword_if,
		keyword_else,
		keyword_elif,
		keyword_while,
		keyword_break,
		keyword_for,
		keyword_return,
		keyword_return_value,
		keyword_task,
		keyword_true,
		keyword_false,
		keyword_none
	};
private:
	WordType type;
public:
	// @owns: raw_string
	Word(IR::Module* _module, const wchar_t * raw_string, int line_index, int column_index, WordType type) :
		Token(_module, raw_string, line_index, column_index), type(type) {
	}
	static shared_ptr<Word> mock(const wchar_t * raw_string)
	{
		wchar_t * copy = new wchar_t[wcslen(raw_string) + 1];
		wcscpy_s(copy, wcslen(raw_string) + 1, raw_string);
		return shared_ptr<Word>(new Word(nullptr, copy, 0, 0, identifier));
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
	const wchar_t* get_word() {
		return this->get_raw_string();
	}
	WordType get_type() {
		return type;
	}

};
