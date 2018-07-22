#include "stdafx.h"

namespace IR
{
	namespace OperandType
	{
		int Label::id = 0;
		Label::Label(wstring && name)
			: name_(name + to_wstring(id++))
		{
		}
		wstring Label::to_string()
		{
			return name_;
		}
	}
}
