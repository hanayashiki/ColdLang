#include "stdafx.h"
#include "RuntimeFunction.h"
#include "BytecodeVectorReader.h"
#include "CompilerFactory.h"
#include "TypedBlockCompilerCaller.h"
#include "CldMem.h"
#include "RuntimeStack.h"
#include "BlockEnv.h"

#include "BytecodeTyper.h"

namespace CldRuntime {
	RuntimeFunction::RuntimeFunction(vector<IR::BasicBlock>* basic_blocks, IR::Function * function)
		: basic_blocks_(basic_blocks), function_(function)
	{
	}

	void RuntimeFunction::Run(const vector<RuntimeStack*> & contexts)
	{
		using IR::SymbolToType;

		BlockEnv env = make_block_env(contexts);
		SymbolToType symbol_to_type;

		vector<IR::BasicBlock>::size_type cur_id = 0;
		while (true)
		{
			IR::BasicBlock & cur_block = basic_blocks_->at(cur_id);
			IR::BytecodeVectorReader reader(&cur_block.get_bytecode_vector());
			// TODO : add typed code
			auto compiler = Compile::CompilerFactory::GetCompiler(&symbol_to_type);
			Compile::TypedBlockCompilerCaller compilerCaller(&reader, compiler.get());
			compilerCaller.NextBlock(symbol_to_type);
			Compile::Code code = compiler->GetCode();

			CLD_DEBUG << "code started" << std::endl;

			code(env);
			lastRetVal = env.block_result->GetPtrToResult();

			CLD_DEBUG << "type: " << ValueTypeName[GetLastRetVal<IntegerValue>()->type] << std::endl;
			CLD_DEBUG << "val: " << int(GetLastRetVal<IntegerValue>()->value) << std::endl;
			// TODO : this is temp
			break;
		}
	}

	BlockEnv RuntimeFunction::make_block_env(const vector<RuntimeStack*> & contexts)
	{
		BlockEnv env;
		env.runtime_stack = make_runtime_stack(function_);
		CLD_DEBUG << "env.runtime_stack = " << env.runtime_stack << std::endl;
		env.block_result = new BlockResult();
		CLD_DEBUG << "env.block_result = " << env.block_result << std::endl;
		env.n_contexts = contexts.size();
		CLD_DEBUG << "env.n_contexts = " << env.n_contexts << std::endl;
		env.contexts = contexts.data();
		CLD_DEBUG << "env.contexts = " << env.contexts << std::endl;
		return env;
	}
	
	RuntimeStack * RuntimeFunction::make_runtime_stack(IR::Function * function)
	{
		const auto symbol_table = function->get_symbol_table();
		CldMem::New<vector<int>>();
		CLD_DEBUG << LOG_EXPR(sizeof(RuntimeStackSlot) * symbol_table->get_variable_count()) << std::endl;
		return CldMem::Malloc<RuntimeStack>(sizeof(RuntimeStackSlot) *
			symbol_table->get_variable_count());
	}
}
