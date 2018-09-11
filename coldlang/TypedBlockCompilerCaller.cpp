#include "stdafx.h"
#include "TypedBlockCompilerCaller.h"
#include "TypeCalc.h"
#include <cassert>
#include "Compiler.h"
#include "MiscellaneousBytecodeInfoGetters.h"
#include "BytecodeTyper.h"

namespace Compile {

	TypedBlockCompilerCaller::TypedBlockCompilerCaller(BytecodeReader * reader, Compiler * compiler)
		: reader_(reader), compiler_(compiler)
	{
	}

	bool TypedBlockCompilerCaller::CompileBinary(BytecodeTyper::BytecodeTypeInfo & type_info, SymbolToType & symbol_to_type)
	{
		assert(type_info.type == BytecodeTyper::BytecodeTypeInfo::Binary);

		const auto & binary_info = type_info.binary;
		if (binary_info.left_type == AnyVal || binary_info.right_type == AnyVal)
		{
			//wcout << "left type: " << ValueTypeName[binary_info.left_type] << endl;
			//wcout << "left: " << to_string(binary_info.left_symbol) << endl;
			//wcout << "right: " << to_string(binary_info.right_symbol) << endl;
			return false;
		}

		symbol_to_type.insert_or_assign(binary_info.target, binary_info.target_type);
		//wcout << "set " << to_string(binary_info.target) << " <- " << ValueTypeName[binary_info.target_type] << endl;
		//wcout << "CompileBinary: symbol to type: " << &symbol_to_type << endl;
		compiler_->CompileBinary(
			binary_info.target, binary_info.op_type,
			binary_info.left_type, binary_info.left_symbol,
			binary_info.right_type, binary_info.right_symbol);
		return true;
	}

	bool TypedBlockCompilerCaller::CompileUnary(BytecodeTyper::BytecodeTypeInfo & type_info, SymbolToType & symbol_to_type)
	{
		assert(type_info.type == BytecodeTyper::BytecodeTypeInfo::Unary);

		const auto & unary_info = type_info.unary;
		if (unary_info.source_type == AnyVal)
		{
			//wcout << "source: " << to_string(unary_info.source) << endl;
			return false;
		}
		symbol_to_type.insert_or_assign(unary_info.target, unary_info.target_type);
		//wcout << "CompileUnary: symbol to type: " << &symbol_to_type << endl;
		compiler_->CompileUnary(unary_info.target, unary_info.op_type, unary_info.source_type, unary_info.source);
		return true;
	}

	bool TypedBlockCompilerCaller::CompileSingle(BytecodeTyper::BytecodeTypeInfo & type_info, SymbolToType & symbol_to_type)
	{
		assert(type_info.type == BytecodeTyper::BytecodeTypeInfo::Single);

		const auto & single_info = type_info.single;
		if (single_info.origin_type == AnyVal)
		{
			return false;
		}
		symbol_to_type.insert_or_assign(single_info.target, single_info.result_type);
		compiler_->CompileSingle(single_info.target, single_info.bytecode_enum, single_info.origin_type);
		return true;
	}

	SymbolToType TypedBlockCompilerCaller::NextBlock(SymbolToType && symbol_to_type)
	{
		return NextBlock(symbol_to_type);
	}

	SymbolToType TypedBlockCompilerCaller::NextBlock(SymbolToType & symbol_to_type)
	{
		unsigned char buf[128];

		while (true)
		{
			// wcout << "start typedblock" << endl;
			// wcout << "get bytecode: " << mem_to_string(buf, read_count).c_str() << endl;
			if (reader_->more() == false)
			{
				break;
			}

			IR::BytecodeTyper::BytecodeTypeInfo type_info =
				BytecodeTyper::get_bytecode_type_info(symbol_to_type, reader_);

			switch (type_info.type)
			{
			case BytecodeTyper::BytecodeTypeInfo::Unary:
				if (!CompileUnary(type_info, symbol_to_type))
				{
					goto while_end;
				}
				break;
			case BytecodeTyper::BytecodeTypeInfo::Binary:
				if (!CompileBinary(type_info, symbol_to_type))
				{
					goto while_end;
				}
				break;
			case BytecodeTyper::BytecodeTypeInfo::Unknown:
				wcout << "Not implemented" << endl;
				assert(false);
				break;
			default:
				assert(false);
			}

		}
		while_end:
		return SymbolToType();
	}
}
