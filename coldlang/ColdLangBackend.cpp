#include "stdafx.h"
#include "ColdLangBackend.h"
#include "BytecodeDumper.h"

ColdLangBackend::ColdLangBackend()
{
	symbol_table_ = new IR::SymbolTable();
	function_table_ = new IR::FunctionTable();
	bytecode_writer_ = new IR::BytecodeDumper();
	ir_gen_ = new IR::IRGenerator(symbol_table_, function_table_, bytecode_writer_);
}

ColdLangBackend::~ColdLangBackend()
{
	delete symbol_table_;
	delete function_table_;
	delete bytecode_writer_;
	delete ir_gen_;
}
