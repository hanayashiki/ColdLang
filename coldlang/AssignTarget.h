#pragma once
#include "stdafx.h"
#include "BytecodeClass.h"
#include "BytecodeWriter.h"
#include "IRGenerator.h"

namespace IR {
	class AssignTarget
	{
	public:
		enum AssignType
		{
			AssignToVariable = 1,
			AssignToProperty
		};
	private:
		OperandType::Variable * symbol_;
		AssignType assign_type_;
	public:
		AssignTarget(OperandType::Variable * symbol) : symbol_(symbol), assign_type_(AssignToVariable)
		{
		}

		void assign(OperandType::Symbol * symbol, BytecodeWriter * bytecode_writer)
		{
			switch (assign_type_)
			{
			AssignToSymbol:
				if (auto func_symbol = dynamic_cast<Function*>(symbol)) {
					EMIT(StoreAcc, bytecode_writer, symbol);
				}
				break;
			default:
				assert(false);
			}
		}
		AssignType get_type()
		{
			return assign_type_;
		}
		Variable * get_target_symbol()
		{
			assert(get_type() == AssignToVariable);
			return symbol_;
		}
	};
}
