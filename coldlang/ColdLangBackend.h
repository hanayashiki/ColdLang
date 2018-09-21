#pragma once
#include "Tree.h"
#include "IRGenerator.h"
#include "BytecodeReader.h"
#include "Compiler.h"
#include "TypedBlockCompilerCaller.h"
#include "DumpingCompiler.h"

class ColdLangBackend
{
private:
public:
	IR::SymbolTable * symbol_table;
	IR::FunctionTable * function_table;
	IR::LiteralTable * literal_table;
	IR::BytecodeWriter * bytecode_dumper;

	vector<uint8_t> bytecode_vector;
	IR::BytecodeWriter * bytecode_vector_writer;
	IR::BytecodeReader * bytecode_reader;
	IR::IRGenerator * ir_gen;

	Compile::TypedBlockCompilerCaller * typed_block_compiler_caller;
	Compile::DumpingCompiler * dump_compiler;

	ColdLangBackend();
	~ColdLangBackend();
};
