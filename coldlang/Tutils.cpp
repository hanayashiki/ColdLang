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
	wstring to_xml_begin_tag(wstring wstr, bool newline)
	{
		return wstring(L"<") + wstr + wstring(L">") + (newline ? L"\n" : L"");
	}
	wstring to_xml_single_tag(wstring wstr, bool newline)
	{
		return wstring(L"<") + wstr + wstring(L"/>") + (newline ? L"\n" : L"");
	}
	wstring to_xml_end_tag(wstring wstr, bool newline)
	{
		return wstring(L"</") + wstr + wstring(L">") + (newline ? L"\n" : L"");
	}
	wstring to_xml_quoted(wstring tag_name, wstring text, bool newline)
	{
		return tutils::to_xml_begin_tag(tag_name, false) + text + tutils::to_xml_end_tag(tag_name, false) + (newline ? L"\n" : L"");
	}
}