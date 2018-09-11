#pragma once
#include "../asmjit/asmjit.h"
#include "BytecodeClass.h"
#include "Compiler.h"

#include "log.h"

namespace Compile
{
	using namespace asmjit;
	using namespace IR;
	using namespace BytecodeClass;
	using namespace CldRuntime;
	class DumpingCompiler : public Compiler
	{
	protected:
		virtual void CompileBinaryImpl(Symbol * target, OpType op_type,
			ValueType left_type, Symbol * left_symbol,
			ValueType right_type, Symbol * right_symbol) override
		{
			CLD_INFO << to_string(target) << L" <- " <<
				ValueTypeName[left_type] << L" " << to_string(left_symbol) <<
				L" " << OpTypeName[op_type] << L" " <<
				ValueTypeName[right_type] << L" " << to_string(right_symbol) << endl;
		}

		virtual void CompileUnaryImpl(Symbol * target, OpType op_type,
			ValueType type, Symbol* left_symbol) override
		{
			CLD_INFO << to_string(target) << L" <- " << OpTypeName[op_type] <<
				L" " << ValueTypeName[type] << L" " << to_string(left_symbol) << endl;
		}

		virtual void CompileSingleImpl(Symbol * target, BytecodeEnum bytecode_name,
			ValueType source_type) override
		{
			CLD_INFO << get_enum_name(bytecode_name) << L" " << ValueTypeName[source_type]
				<< L" " << to_string(target);
		}

		virtual Code GetCode() override
		{
			return Code(nullptr);
		}
	};
}