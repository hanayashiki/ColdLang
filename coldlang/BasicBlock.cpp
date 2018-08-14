#include "stdafx.h"
#include "BasicBlock.h"
#include "BytecodeVectorReader.h"

namespace IR {
	BasicBlock::BasicBlock(Label && label)
		: label_(label), does_link_(false), link_(0)
	{
		strcpy(name, "basicblock");
	}

	BasicBlock::BasicBlock(Label & label)
		: label_(label), does_link_(false), link_(0)
	{
		strcpy(name, "basicblock");
	}

	BasicBlock::BasicBlock(const BasicBlock & block)
		: label_(block.label_), does_link_(block.does_link_), link_(block.link_)
	{
		bytecode_vector_ = block.bytecode_vector_;
	}

	BasicBlock::BasicBlock(BasicBlock && block) noexcept
		: label_(std::move(block.label_)), does_link_(block.does_link_), link_(block.link_)
	{
		bytecode_vector_ = std::move(block.bytecode_vector_);
	}

	BasicBlock & BasicBlock::operator=(BasicBlock && block) noexcept
	{
		label_ = block.label_;
		bytecode_vector_ = std::move(block.bytecode_vector_);
		does_link_ = block.does_link_;
		link_ = block.link_;
		return *this;
	}

	BasicBlock & BasicBlock::operator=(const BasicBlock & block)
	{
		label_ = block.label_;
		bytecode_vector_ = block.bytecode_vector_;
		does_link_ = block.does_link_;
		link_ = block.link_;
		return *this;
	}

	wstring BasicBlock::to_string()
	{
		wstring display = L"---------------Block '" + label_.to_string() + L"' begin---------------\n";
		BytecodeVectorReader vec_reader(&get_bytecode_vector());
		while (vec_reader.more())
		{
			size_t count;
			unsigned char buf[128];
			vec_reader.read_byte(buf, &count);
			display += BytecodeClass::decompile(buf) + L"\n";
		}
		display += L"---------------Block '" + label_.to_string() + L"' end-----------------\n\n";
		return display;
	}

	vector<uint8_t> & BasicBlock::get_bytecode_vector()
	{
		return bytecode_vector_;
	}

	Label & BasicBlock::get_label()
	{
		return label_;
	}

	void BasicBlock::set_link(const vector<BasicBlock>::size_type & link)
	{
		does_link_ = true;
		link_ = link;
	}
}

