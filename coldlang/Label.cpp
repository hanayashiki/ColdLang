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
		Label::Label(const Label & label)
		{
			id = label.id;
			name_ = label.name_;
		}
		Label::Label(Label && label) noexcept
		{
			id = label.id;
			name_ = std::move(label.name_);
		}
		Label & Label::operator=(Label && label) noexcept
		{
			id = label.id;
			name_ = std::move(label.name_);
			return *this;
		}
		Label & Label::operator=(const Label & label)
		{
			id = label.id;
			name_ = label.name_;
			return *this;
		}
		wstring Label::to_string()
		{
			return name_;
		}
	}
}
