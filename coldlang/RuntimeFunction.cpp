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

	void RuntimeFunction::run(const vector<RuntimeStack*> & contexts)
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
			code(env);
			// TODO : this is temp
			break;
		}
	}

	BlockEnv RuntimeFunction::make_block_env(const vector<RuntimeStack*> & contexts)
	{
		BlockEnv env;
		env.runtime_stack = make_runtime_stack(function_);
		// TODO: block_result ?
		env.n_contexts = contexts.size();
		env.contexts = contexts.data();
		return env;
	}
	
	RuntimeStack * RuntimeFunction::make_runtime_stack(IR::Function * function)
	{
		const auto symbol_table = function->get_symbol_table();
		CldMem::New<vector<int>>();
		return CldMem::Malloc<RuntimeStack>(sizeof(RuntimeStackSlot) *
			symbol_table->get_variable_count());
	}
}
