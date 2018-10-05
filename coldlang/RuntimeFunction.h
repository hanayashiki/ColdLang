#pragma once
#include "log.h"

#include "BasicBlock.h"
#include "BlockCode.h"
#include "BlockEnv.h"

namespace CldRuntime
{
	class RuntimeFunction
	{
	public:
		RuntimeFunction(vector<IR::BasicBlock> * basic_blocks, IR::Function * function);
		volatile void Run(const vector<RuntimeStack*> & contexts);
		template<typename T> T * GetLastRetVal() 
		{
			std::wcerr << LOG_EXPR(this) << std::endl;
			std::wcerr << LOG_EXPR(this->lastRetVal) << std::endl;
			CLD_DEBUG << LOG_EXPR(this->lastRetVal) << std::endl;
			return reinterpret_cast<T*>(lastRetVal); 
		}

		~RuntimeFunction();
	private:
		vector<IR::BasicBlock> * basic_blocks_;
		IR::Function * function_ = nullptr;
		RuntimeValue * lastRetVal = nullptr;

		BlockEnv make_block_env(const vector<RuntimeStack*> & contexts);
		static RuntimeStack * make_runtime_stack(IR::Function * function);
	};
}
