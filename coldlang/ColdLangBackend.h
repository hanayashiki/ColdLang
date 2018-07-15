#pragma once
#include "Tree.h"
#include "IRGenerator.h"

class ColdLangBackend
{
private:
public:
	IR::IRGenerator * ir_gen_;
	IR::SymbolTable * symbol_table;
	IR::BytecodeWriter * bytecode_writer_;

	ColdLangBackend(Tree * tree);
	~ColdLangBackend();
};
