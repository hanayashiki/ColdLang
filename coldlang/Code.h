#pragma once
#include "Variable.h"
#include "BlockEnv.h"
#include "RuntimeStack.h"
#include "BlockResult.h"
#include "../asmjit/asmjit.h"

namespace Compile
{

	class Code
	{
	public:
		typedef void(*FuncPtr)(CldRuntime::BlockEnv);
		inline const static asmjit::FuncSignature AsmjitFuncSignature =
			asmjit::FuncSignature4<uint64_t, 
				CldRuntime::RuntimeStack *, 
				CldRuntime::BlockResult *,
				size_t, 
				CldRuntime::RuntimeStack* const*>();

		static IR::Variable RuntimeStackPtr;
		static IR::Variable BlockResultPtr;
		static IR::Variable NContexts;
		static IR::Variable ContextsPtr;

	private:
		FuncPtr func_ptr_;
	public:
		explicit Code(FuncPtr func_ptr)
			: func_ptr_(func_ptr)
		{
		}
		~Code();

		void operator() (CldRuntime::BlockEnv env)
		{
			reinterpret_cast<FuncPtr>(func_ptr_)(env);
		}
	};
}
