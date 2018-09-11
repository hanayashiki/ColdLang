#pragma once
#include "BasicBlock.h"
#include "BlockCode.h"
#include "BlockEnv.h"

namespace CldRuntime
{
	class RuntimeFunction
	{
	public:
		RuntimeFunction(vector<IR::BasicBlock> * basic_blocks, IR::Function * function);
		void run(const vector<RuntimeStack*> & contexts);
	private:
		vector<IR::BasicBlock> * basic_blocks_;
		IR::Function * function_;

		BlockEnv make_block_env(const vector<RuntimeStack*> & contexts);
		static RuntimeStack * make_runtime_stack(IR::Function * function);
	};
}
