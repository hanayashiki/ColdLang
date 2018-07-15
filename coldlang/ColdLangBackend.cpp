#include "stdafx.h"
#include "ColdLangBackend.h"
#include "BytecodeDumper.h"

ColdLangBackend::ColdLangBackend(Tree * tree)
{
	symbol_table = new IR::SymbolTable();
	bytecode_writer_ = new IR::BytecodeDumper();
	ir_gen_ = new IR::IRGenerator(tree, symbol_table, bytecode_writer_);
}

ColdLangBackend::~ColdLangBackend()
{
	delete symbol_table;
	delete bytecode_writer_;
	delete ir_gen_;
}
