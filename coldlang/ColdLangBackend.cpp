#include "stdafx.h"
#include "ColdLangBackend.h"
#include "BytecodeDumper.h"
#include "BytecodeVectorReader.h"
#include "BytecodeVectorWriter.h"

ColdLangBackend::ColdLangBackend()
{
	symbol_table = new IR::SymbolTable();
	function_table = new IR::FunctionTable();
	literal_table = new IR::LiteralTable();
	constant_table = new IR::ConstantTable();
	bytecode_dumper = new IR::BytecodeDumper();

	bytecode_vector_writer = new IR::BytecodeVectorWriter(&bytecode_vector);
	bytecode_dumper->pass_to(bytecode_vector_writer);
	bytecode_reader = 
		new IR::BytecodeVectorReader(&bytecode_vector);

	dump_compiler = new Compile::DumpingCompiler();
	typed_block_compiler_caller = new Compile::TypedBlockCompilerCaller(bytecode_reader, dump_compiler);

	ir_gen = new IR::IRGenerator(
		symbol_table,
		function_table,
		literal_table,
		constant_table,
		bytecode_dumper
	);
}

ColdLangBackend::~ColdLangBackend()
{
	delete symbol_table;
	delete function_table;
	delete literal_table;
	delete constant_table;
	delete bytecode_dumper;
	delete bytecode_vector_writer;
	delete bytecode_reader;
	delete ir_gen;
	delete dump_compiler;
	delete typed_block_compiler_caller;
}
