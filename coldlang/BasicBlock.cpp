#include "stdafx.h"
#include "BasicBlock.h"

namespace IR {
	BasicBlock::BasicBlock(Label && label)
		: label_(label)
	{
	}

	BasicBlock::BasicBlock(Label & label)
		: label_(label)
	{
	}
	BasicBlock::BasicBlock(const BasicBlock & block)
		: label_(block.label_)
	{
		bytecode_vector_ = block.bytecode_vector_;
	}
	BasicBlock::BasicBlock(BasicBlock && block) noexcept
		: label_(std::move(block.label_))
	{
		bytecode_vector_ = std::move(block.bytecode_vector_);
	}
	BasicBlock & BasicBlock::operator=(BasicBlock && block) noexcept
	{
		label_ = block.label_;
		bytecode_vector_ = std::move(block.bytecode_vector_);
		return *this;
	}
	BasicBlock & BasicBlock::operator=(const BasicBlock & block)
	{
		label_ = block.label_;
		bytecode_vector_ = block.bytecode_vector_;
		return *this;
	}
	vector<uint8_t>* BasicBlock::get_bytecode_vector()
	{
		return &bytecode_vector_;
	}
	Label BasicBlock::get_label()
	{
		return label_;
	}
	BasicBlock::~BasicBlock()
	{
	}
}

