#pragma once
#include "Tree.h"
#include "IRGenerator.h"

class ColdLangBackend
{
private:
public:
	IR::IRGenerator * ir_gen_;
	IR::SymbolTable * symbol_table_;
	IR::FunctionTable * function_table_;
	IR::BytecodeWriter * bytecode_writer_;

	ColdLangBackend();
	~ColdLangBackend();
};
