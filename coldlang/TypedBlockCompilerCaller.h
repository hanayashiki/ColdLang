#pragma once
#include "BytecodeReader.h"
#include "BasicBlock.h"
#include "Compiler.h"
#include "BytecodeTyper.h"


namespace Compile
{
	using namespace IR;
	using namespace IR::BytecodeClass;
	using namespace CldRuntime;

	class TypedBlockCompilerCaller
	{
	public:
		TypedBlockCompilerCaller(BytecodeReader * reader, Compiler * compiler);
		SymbolToType NextBlock(SymbolToType & symbol_to_type);
		SymbolToType NextBlock(SymbolToType && symbol_to_type = SymbolToType());
	private:
		bool CompileBinary(BytecodeTyper::BytecodeTypeInfo & type_info, SymbolToType & symbol_to_type);
		bool CompileUnary(BytecodeTyper::BytecodeTypeInfo & type_info, SymbolToType & symbol_to_type);
		bool CompileSingle(BytecodeTyper::BytecodeTypeInfo & type_info, SymbolToType & symbol_to_type);

		function<ValueType(Symbol*)> get_type_of_;
		BytecodeReader * reader_;
		Compiler * compiler_;
	};
}
 
