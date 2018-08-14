#pragma once
#include <vector>
#include "Label.h"

namespace IR {
	using namespace std;
	class BasicBlock
	{
	private:
		char name[20];
		OperandType::Label label_;
		vector<uint8_t> bytecode_vector_;
		bool does_link_;
		vector<BasicBlock>::size_type link_;
	public:
		explicit BasicBlock(OperandType::Label &&);
		explicit BasicBlock(OperandType::Label &);
		BasicBlock(const BasicBlock &);
		BasicBlock(BasicBlock &&) noexcept;
		BasicBlock & operator=(BasicBlock&&) noexcept;
		BasicBlock & operator=(const BasicBlock&);

		wstring to_string();

		vector<uint8_t> & get_bytecode_vector();
		OperandType::Label & get_label();
		void set_link(const vector<BasicBlock>::size_type & link);
	};
}