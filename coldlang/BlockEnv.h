#pragma once
#include "BlockResult.h"
#include "RuntimeStack.h"
#include "../asmjit/asmjit.h"

namespace CldRuntime {
	struct RuntimeStack;
	struct BlockResult;

	struct BlockEnv {
		inline const static asmjit::X86Gp & runtime_stack_reg 
			= asmjit::x86::rcx;
		RuntimeStack * runtime_stack = nullptr; // to rcx

		inline const static asmjit::X86Gp & block_result_reg 
			= asmjit::x86::rdx;
		BlockResult * block_result = nullptr; // to rdx

		inline const static asmjit::X86Gp & n_contexts_reg
			= asmjit::x86::r8;
		size_t n_contexts = 0; // to r8

		inline const static asmjit::X86Gp & contexts_reg
			= asmjit::x86::r9;
		RuntimeStack* const* contexts = nullptr; // to r9
	};
}