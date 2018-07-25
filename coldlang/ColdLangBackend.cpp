#include "stdafx.h"
#include "ColdLangBackend.h"
#include "BytecodeDumper.h"
#include "BytecodeVectorReader.h"
#include "BytecodeVectorWriter.h"

ColdLangBackend::ColdLangBackend()
{
	symbol_table_ = new IR::SymbolTable();
	function_table_ = new IR::FunctionTable();
	literal_table_ = new IR::LiteralTable();
	constant_table_ = new IR::ConstantTable();
	bytecode_dumper_ = new IR::BytecodeDumper();

	bytecode_byte_dump_ = new IR::BytecodeVectorWriter(&bytecode_vector_);
	bytecode_dumper_->pass_to(bytecode_byte_dump_);
	bytecode_reader_ = 
		new IR::BytecodeVectorReader(&bytecode_vector_);

	ir_gen_ = new IR::IRGenerator(
		symbol_table_,
		function_table_,
		literal_table_,
		constant_table_,
		bytecode_dumper_
	);
}

ColdLangBackend::~ColdLangBackend()
{
	delete symbol_table_;
	delete function_table_;
	delete literal_table_;
	delete constant_table_;
	delete bytecode_dumper_;
	delete bytecode_byte_dump_;
	delete bytecode_reader_;
	delete ir_gen_;
}
