#include "stdafx.h"

namespace tutils
{
	TreeUnitBuilder optional(TreeUnitBuilder tub)
	{
		tub.set_optional(true);
		return tub;
	}
	TreeUnitBuilder negative(TreeUnitBuilder tub)
	{
		tub.set_not(true);
		return tub;
	}
	TreeUnitBuilder logical_or(initializer_list<TreeUnitBuilder> && tubs)
	{
		return TreeUnitBuilder(tubs);
	}

	wstring ascii_string_to_wstring(const string str)
	{
		const auto result = new wchar_t[str.length() + 1];
		int i;
		for (i = 0; i < str.length(); i++)
		{
			result[i] = wchar_t(str[i]);
		}
		result[i] = L'\0';
		auto r = wstring(result);
		delete[] result;
		return r;
	}
	wstring get_indent(int indent, int indent_size)
	{
		wstring res;
		res.reserve(16);
		for (int i = 0; i < indent * indent_size; i++)
		{
			res.push_back(' ');
		}
		return res;
	}
	wstring self_or_shorter(wstring & wstr, int max_char)
	{
		return (wstr.length() < max_char ? wstr : wstr.substr(0, max_char));
	}
	wstring to_xml_begin_tag(wstring wstr, int max_char, bool newline)
	{
		return wstring(L"<") + self_or_shorter(wstr, max_char) + wstring(L">") + (newline ? L"\n" : L"");
	}
	wstring to_xml_single_tag(wstring wstr, int max_char, bool newline)
	{
		return wstring(L"<") + self_or_shorter(wstr, max_char) + wstring(L"/>") + (newline ? L"\n" : L"");
	}
	wstring to_xml_end_tag(wstring wstr, int max_char, bool newline)
	{
		return wstring(L"</") + self_or_shorter(wstr, max_char) + wstring(L">") + (newline ? L"\n" : L"");
	}
	wstring to_xml_quoted(wstring tag_name, wstring text, int max_char, bool newline)
	{
		return tutils::to_xml_begin_tag(tag_name, max_char, false) + text + tutils::to_xml_end_tag(tag_name, max_char, false) + (newline ? L"\n" : L"");
	}
}