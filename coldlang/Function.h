#pragma once
#include "stdafx.h"
#include "Variable.h"
#include "BytecodeReader.h"
#include "BytecodeWriter.h"
#include "SymbolTable.h"

#include <functional>

namespace IR
{
	/* function links to definition of a CldRuntime::FunctionObject */
	class Function : public CLObject
	{
	private:
		shared_ptr<Token> token_;
		vector<OperandType::Variable*> parameter_list_;
		vector<uint8_t> bytecode_;
		BytecodeReader * bytecode_reader_;
		BytecodeWriter * bytecode_writer_;
		BytecodeWriter * bytecode_byte_writer;
		SymbolTable * symbol_table_;
	public:
		bool bytecode_compiled;
		function<void()> compile_to_bytecode;

		Function(shared_ptr<Token> & token, vector<OperandType::Variable *> & parameter_list, SymbolTable * symbol_table);
		virtual ~Function();

		wstring to_string() override;
		BytecodeReader * get_bytecode_reader();
		BytecodeWriter * get_bytecode_writer();

		const SymbolTable * get_symbol_table();

	};
};