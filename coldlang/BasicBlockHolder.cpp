#include "stdafx.h"
#include "BasicBlockHolder.h"
#include "BytecodeClassHelper.h"
#include "BytecodeTyper.h"

#include "log.h"

namespace IR {

	BasicBlockHolder::BasicBlockHolder(BytecodeReader * reader)
		: reader_(reader)
	{
		//wcout << L"reader: " << reader_ << endl;
	}

	void BasicBlockHolder::make_blocks()
	{
		//wcout << L"reader: " << reader_ << endl;
		SymbolToType symbol_to_type;
		auto block_and_id = register_block_(L"start");
		BasicBlock * current_block = std::get<0>(block_and_id);
		label_seq_start_ = true;
		set_label_seq_block_id(std::get<1>(block_and_id));

#define NEW_BLOCK(block_name) \
	{ \
		blocks_.push_back(std::move(*current_block)); \
		delete current_block; \
		auto block_and_id = register_block_(block_name); \
		current_block = std::get<0>(block_and_id); \
	} 

		bool skip_until_next_label = false;
		while (reader_->more())
		{
			size_t bytecode_len;
			unsigned char bytecode_buf[128];
			reader_->read_byte(bytecode_buf, &bytecode_len);

			if (!is_label(bytecode_buf))
			{
				label_seq_start_ = false;
			}

			if (skip_until_next_label)
			{
				if (!is_label(bytecode_buf)) 
				{
					continue;
				}
				else 
				{
					skip_until_next_label = false;
				}
			}

			BytecodeTyper::BytecodeTypeInfo type_info =
				BytecodeTyper::get_bytecode_type_info(symbol_to_type, bytecode_buf);
			switch (type_info.type) {
			case BytecodeTyper::BytecodeTypeInfo::Unary:
			{
				ValueType src_type = type_info.unary.source_type;
				/*
				if (src_type == AnyVal) {
					NEW_BLOCK(L"typed")
				}*/
				symbol_to_type.insert_or_assign(type_info.unary.target, type_info.unary.target_type);
			}
			break;
			case BytecodeTyper::BytecodeTypeInfo::Binary:
			{
				ValueType left_type = type_info.binary.left_type;
				ValueType right_type = type_info.binary.right_type;
				/*
				if (left_type == AnyVal || right_type == AnyVal)
				{
					NEW_BLOCK(L"typed")
				}*/
				symbol_to_type.insert_or_assign(type_info.binary.target, type_info.binary.target_type);
			}
			break;
			case BytecodeTyper::BytecodeTypeInfo::Jump:
			{
				auto id = type_info.jump.label_id;
				auto block_id = label_to_block_[id];
				current_block->set_link(block_id);
				symbol_to_type.clear();
				NEW_BLOCK(L"jumped")
			}
			break;
			case BytecodeTyper::BytecodeTypeInfo::Label:
			{
				// wcout << "see: " << decompile(bytecode_buf) << endl;
				if (!label_seq_start_)
				{
					blocks_.push_back(std::move(*current_block));
					auto block_and_id =
						register_block_(OperandType::Label::id_to_label(type_info.label.label_id).to_string().c_str());
					delete current_block;
					current_block = std::get<0>(block_and_id);
					set_label_seq_block_id(std::get<1>(block_and_id));
					label_seq_start_ = true;
				}
				else
				{
					label_to_block_.insert(std::make_pair(type_info.label.label_id, get_label_seq_block_id()));
				}
			}
			case BytecodeTyper::BytecodeTypeInfo::Single:
			{
				ValueType origin_type = type_info.single.origin_type;
				ValueType target_type = type_info.single.result_type;
				switch (type_info.single.bytecode_enum) {
				case EnumRetAcc:
					skip_until_next_label = true; // Clean dead code
					break;
				default:
					CLD_DEBUG << "Single switch used default" << std::endl;
				}
			}
			break;
			default:
				assert(false);
			}

			add_code_(bytecode_buf, bytecode_len, *current_block);
		}

		blocks_.push_back(std::move(*current_block));
		delete current_block;
	}

	wstring BasicBlockHolder::to_string()
	{
		wstring ret;
		for (auto & block : blocks_)
		{
			ret += block.to_string();
		}
		return ret;
	}

	vector<BasicBlock>& BasicBlockHolder::get_blocks()
	{
		return blocks_;
	}

	BasicBlockHolder::~BasicBlockHolder()
	{
	}

	void BasicBlockHolder::add_code_(const unsigned char buf[], size_t count, BasicBlock & basicblock)
	{
		auto & vec = basicblock.get_bytecode_vector();
		vec.insert(vec.end(), buf, buf + count);
	}

	std::tuple<BasicBlock*, vector<BasicBlock>::size_type> BasicBlockHolder::register_block_(const wchar_t* name_)
	{
		OperandType::Label label(name_);
		auto id = blocks_.size();
		label_to_block_.insert(std::make_pair(label.get_id(), id));
		return std::make_tuple(new BasicBlock(label), id);
	}

	void BasicBlockHolder::set_label_seq_block_id(vector<BasicBlock>::size_type id)
	{
		label_seq_block_id_ = id;
	}

	vector<BasicBlock>::size_type & BasicBlockHolder::get_label_seq_block_id()
	{
		return label_seq_block_id_;
	}

}
