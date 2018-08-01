#pragma once
#include <vector>
#include "Label.h"

namespace IR {
	using namespace std;
	class BasicBlock
	{
	private:
		Label label_;
		vector<uint8_t> bytecode_vector_;
	public:
		explicit BasicBlock(Label &&);
		explicit BasicBlock(Label &);
		BasicBlock(const BasicBlock &);
		BasicBlock(BasicBlock &&);
		BasicBlock & operator=(BasicBlock&&);
		BasicBlock & operator=(const BasicBlock&);

		vector<uint8_t> * get_bytecode_vector();
		Label get_label();
		~BasicBlock();
	};
}