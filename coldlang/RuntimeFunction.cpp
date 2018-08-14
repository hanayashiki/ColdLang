#include "stdafx.h"
#include "RuntimeFunction.h"
#include "BytecodeVectorReader.h"
#include "CompilerFactory.h"
#include "TypedBlockCompilerCaller.h"

namespace CldRuntime {
	RuntimeFunction::RuntimeFunction(vector<IR::BasicBlock>* basic_blocks, IR::Function * function)
		: basic_blocks_(basic_blocks), function_(function)
	{
		vector<IR::BasicBlock>::size_type id = 0;
		for (auto basicblock : *basic_blocks)
		{
			block_codes_.emplace_back(id);
			id++;
		}
	}

	void RuntimeFunction::run()
	{
		vector<IR::BasicBlock>::size_type cur_id = 0;
		while (true)
		{
			IR::BasicBlock & cur_block = basic_blocks_->at(cur_id);
			IR::BytecodeVectorReader reader(&cur_block.get_bytecode_vector());
			// TODO : add typed code
			auto compiler = Compile::CompilerFactory::GetCompiler();
			Compile::TypedBlockCompilerCaller compilerCaller(&reader, compiler.get());
			compilerCaller.NextBlock();
			Compile::Code code = compiler->GetCode();
			code();
			// TODO : this is temp
			break;
		}
	}


}
