#pragma once
#include <vector>
#include "BasicBlock.h"

namespace IR {
	class BasicBlockHolder
	{
	public:
		explicit BasicBlockHolder(BytecodeReader * reader);
		void make_blocks();
		wstring to_string();
		vector<BasicBlock> & get_blocks();

		~BasicBlockHolder();
		BasicBlockHolder(const BasicBlock &) = delete;
		BasicBlockHolder(BasicBlock &&) = delete;
		BasicBlockHolder & operator=(BasicBlockHolder&&) = delete;
		BasicBlockHolder & operator=(const BasicBlockHolder&) = delete;
	private:
		unordered_map<OperandType::Label::id_type, vector<BasicBlock>::size_type>
			label_to_block_;
		vector<BasicBlock> blocks_;
		BytecodeReader * reader_;

		void add_code_(const unsigned char buf[], size_t count, BasicBlock & basicblock);
		std::tuple<BasicBlock*, vector<BasicBlock>::size_type>
			register_block_(const wchar_t * name_);
		void set_label_seq_block_id(vector<BasicBlock>::size_type id);
		vector<BasicBlock>::size_type & get_label_seq_block_id();
		vector<BasicBlock>::size_type label_seq_block_id_;
		bool label_seq_start_ = false;
	};
}