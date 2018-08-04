#pragma once
#include "BytecodeReader.h"
#include "BasicBlock.h"
#include "Compiler.h"


namespace Compile
{
	using namespace IR;
	using namespace IR::BytecodeClass;
	using namespace Runtime;

	typedef unordered_map<Symbol*, ValueType> 
		BlockTypeInfo;
	class TypedBlockCompilerCaller
	{
	public:
		TypedBlockCompilerCaller(BytecodeReader * reader, Compiler * compiler);
		BlockTypeInfo next_block(BlockTypeInfo & type_info);
		BlockTypeInfo next_block(BlockTypeInfo && type_info = BlockTypeInfo());
	private:
		bool compile_unary(unsigned char bytecode_buf[], BlockTypeInfo & type_info);
		bool compile_store_acc(unsigned char bytecode_buf[], BlockTypeInfo & type_info);

		function<ValueType(Symbol*)> get_get_type_of(BlockTypeInfo & type_info);
		function<ValueType(Symbol*)> get_type_of_;
		BytecodeReader * reader_;
		Compiler * compiler_;
	};
}

