#pragma once
#include "Tree.h"
#include "IRGenerator.h"
#include "BytecodeReader.h"

class ColdLangBackend
{
private:
public:
	IR::SymbolTable * symbol_table_;
	IR::FunctionTable * function_table_;
	IR::LiteralTable * literal_table_;
	IR::ConstantTable * constant_table_;
	IR::BytecodeWriter * bytecode_dumper_;

	vector<uint8_t> bytecode_vector_;
	IR::BytecodeWriter * bytecode_byte_dump_;
	IR::BytecodeReader * bytecode_reader_;
	IR::IRGenerator * ir_gen_;

	ColdLangBackend();
	~ColdLangBackend();
};
