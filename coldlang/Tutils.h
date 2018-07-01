#pragma once
#include "stdafx.h"

class TreeUnitBuilder;

namespace tutils
{
	TreeUnitBuilder optional(TreeUnitBuilder tub);
	TreeUnitBuilder negative(TreeUnitBuilder tub);
	wstring ascii_string_to_wstring(const string str);
	wstring get_indent(int indent, int indent_size);
	wstring to_xml_begin_tag(wstring wstr, bool newline = true);
	wstring to_xml_single_tag(wstring wstr, bool newline = true);
	wstring to_xml_end_tag(wstring wstr, bool newline = true);
	wstring to_xml_quoted(wstring tag_name, wstring text, bool newline = true);
}
