#pragma once
#include "../asmjit/asmjit.h"
#include "BytecodeClass.h"
#include "Compiler.h"

namespace Compile
{
	using namespace asmjit;
	using namespace IR;
	using namespace BytecodeClass;
	using namespace Runtime;
	class DumpingCompiler : public Compiler
	{
	public:
		virtual void CompileBinary(Symbol * target, OpType op_type,
			ValueType left_type, Symbol * left_symbol,
			ValueType right_type, Symbol * right_symbol) override
		{
			wcout << to_string(target) << L" <- " <<
				ValueTypeName[left_type] << L" " << to_string(left_symbol) <<
				L" " << OpTypeName[op_type] << L" " <<
				ValueTypeName[right_type] << L" " << to_string(right_symbol) << endl;
		}

		virtual void CompileUnary(Symbol * target, OpType op_type,
			ValueType type, Symbol* left_symbol)
		{
			wcout << to_string(target) << L" <- " << OpTypeName[op_type] <<
				L" " << ValueTypeName[type] << L" " << to_string(left_symbol) << endl;
		}
	};
}