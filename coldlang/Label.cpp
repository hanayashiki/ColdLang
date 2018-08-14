#include "stdafx.h"

namespace IR
{
	namespace OperandType
	{
		vector<Label> Label::label_table_;
		uint32_t Label::global_id_ = 0;
		Label::Label(wstring && name)
			: name_(name + to_wstring(global_id_)), id_(global_id_)
		{
			label_table_.push_back(*this);
			global_id_++;
		}
		Label::Label(const Label & label)
		{
			id_ = label.id_;
			name_ = label.name_;
		}
		Label::Label(Label && label) noexcept
		{
			id_ = label.id_;
			name_ = std::move(label.name_);
		}
		Label & Label::operator=(Label && label) noexcept
		{
			id_ = label.id_;
			name_ = std::move(label.name_);
			return *this;
		}
		Label & Label::operator=(const Label & label)
		{
			id_ = label.id_;
			name_ = label.name_;
			return *this;
		}
		wstring Label::to_string()
		{
			return name_;
		}
		const uint32_t & Label::get_id()
		{
			return id_;
		}
		Label & Label::id_to_label(uint32_t id)
		{
			return label_table_.at(id);
		}
	}
}
