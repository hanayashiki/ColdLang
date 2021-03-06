#include "stdafx.h"
#include "FunctionTable.h"
#include "BytecodeVectorReader.h"
#include "BytecodeVectorWriter.h"
#include "BytecodeDumper.h"

namespace IR
{
	Function::Function(shared_ptr<Token>& token, vector<Variable*>& parameter_list, SymbolTable * symbol_table)
		: token_(token), parameter_list_(parameter_list), symbol_table_(symbol_table),
		bytecode_reader_(new BytecodeVectorReader(&bytecode_)),
		bytecode_byte_writer(new BytecodeVectorWriter(&bytecode_)),
		bytecode_writer_(new BytecodeDumper()),
		bytecode_compiled(false)
	{
		bytecode_writer_->pass_to(bytecode_byte_writer);
	}

	wstring IR::Function::to_string()
	{
		return L"function";
	}

	BytecodeReader * Function::get_bytecode_reader()
	{
		return bytecode_reader_;
	}

	BytecodeWriter * Function::get_bytecode_writer()
	{
		return bytecode_writer_;
	}

	const SymbolTable * Function::get_symbol_table()
	{
		return symbol_table_;
	}

	Function::~Function()
	{
		delete bytecode_writer_;
		delete bytecode_byte_writer;
		delete bytecode_reader_;
	}

}
