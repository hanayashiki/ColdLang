#pragma once
#include "../asmjit/asmjit.h"
#include "BytecodeClass.h"

namespace Compile
{
	using namespace asmjit;
	using namespace IR;
	using namespace BytecodeClass;
	using namespace Runtime;
	class Compiler
	{
	public:
		virtual void CompileBinary(Symbol * target, OpType op_type, 
			ValueType left_type, Symbol * left_symbol, 
			ValueType right_type, Symbol * right_symbol) = 0;
		virtual void CompileUnary(Symbol * target, OpType op_type,
			ValueType type, Symbol* left_symbol) = 0;
	};
}