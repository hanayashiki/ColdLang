#pragma once
#include "BasicBlock.h"
#include "TypeToCode.h"
#include "BlockCode.h"

namespace CldRuntime
{
	class RuntimeFunction
	{
	private:
		vector<IR::BasicBlock> * basic_blocks_;
		vector<BlockCode> block_codes_;
		IR::Function * function_;
	public:
		RuntimeFunction(vector<IR::BasicBlock> * basic_blocks, IR::Function * function);
		void run();
	};
}
