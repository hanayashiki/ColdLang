#include "stdafx.h"
#include "TypedBlockCompilerCaller.h"
#include "TypeCalc.h"
#include <cassert>
#include "Compiler.h"
#include "MiscellaneousBytecodeInfoGetters.h"

namespace Compile {

	TypedBlockCompilerCaller::TypedBlockCompilerCaller(BytecodeReader * reader, Compiler * compiler)
		: reader_(reader), compiler_(compiler)
	{
	}

	BlockTypeInfo TypedBlockCompilerCaller::next_block(BlockTypeInfo && type_info)
	{
		BlockTypeInfo info = type_info;
		return next_block(type_info);
	}

	bool TypedBlockCompilerCaller::compile_unary(unsigned char bytecode_buf[], BlockTypeInfo & type_info)
	{
		GetUnaryInfo get_unary_info = get_get_unary_info(bytecode_buf);
		UnaryInfo unary_info = get_unary_info(bytecode_buf);
		OpType op_type = unary_info.op;
		Symbol* target_symbol = Symbol::Acc;
		Symbol* left_symbol = Symbol::Acc;
		ValueType left_type = get_type_of_(left_symbol);
		Symbol* right_symbol = unary_info.symbol;
		ValueType right_type = get_type_of_(right_symbol);

		if (op_type == OpMove)
		{
			left_type = right_type;
			left_symbol = right_symbol;
		}

		if (left_type == AnyVal || right_type == AnyVal)
		{
			// wcout << "left type: " << ValueTypeName[left_type] << endl;
			// wcout << "left: " << to_string(left_symbol) << endl;
			// wcout << "right: " << to_string(right_symbol) << endl;
			return false;
		}

		ValueType target_type = TypeCalc::get_result_type(op_type, left_type, right_type);
		type_info.insert_or_assign(target_symbol, target_type);
		// wcout << "set " << to_string(target_symbol) << " <- " << ValueTypeName[target_type] << endl;
		if (op_type != OpMove) {
			compiler_->CompileBinary(
				target_symbol, op_type,
				left_type, left_symbol,
				right_type, right_symbol);
		} else
		{
			compiler_->CompileUnary(
				target_symbol, op_type,
				left_type, left_symbol);
		}
		return true;
	}

	bool TypedBlockCompilerCaller::compile_store_acc(unsigned char bytecode_buf[], BlockTypeInfo & type_info)
	{
		UnaryInfo unary_info = get_unary_info_StoreAcc(bytecode_buf);
		ValueType acc_type = get_type_of_(Symbol::Acc);
		if (acc_type == AnyVal)
		{
			return false;
		}
		type_info.insert_or_assign(unary_info.symbol, acc_type);
		compiler_->CompileUnary(unary_info.symbol, unary_info.op, acc_type, Symbol::Acc);
		return true;
	}

	BlockTypeInfo TypedBlockCompilerCaller::next_block(BlockTypeInfo & type_info)
	{
		unsigned char buf[128];

		get_type_of_ = get_get_type_of(type_info);
		while (true)
		{
			// wcout << "start typedblock" << endl;
			size_t read_count;
			reader_->read_byte(buf, &read_count);
			// wcout << "get bytecode: " << mem_to_string(buf, read_count).c_str() << endl;
			if (read_count == 0)
			{
				break;
			}
			if (is_unary(buf))
			{
				if (!compile_unary(buf, type_info))
				{
					goto while_end;
				}
			} else
			{
				switch(buf[0])
				{
				case EnumStoreAcc:
				if (!compile_store_acc(buf, type_info))
				{
					goto while_end;
				}
				break;
				default:
					wcout << "Not implemented. " << endl;
				}
			}

		}
		while_end:
		return BlockTypeInfo();
	}


	function<ValueType(Symbol*)> TypedBlockCompilerCaller::get_get_type_of(BlockTypeInfo & type_info)
	{
		return [&](Symbol* symbol) -> ValueType
		{
			if (type_info.find(symbol) != type_info.end())
			{
				return type_info.find(symbol)->second;
			}
			else if (auto constant = dynamic_cast<Constant*>(symbol))
			{
				//wcout << "constant: " << constant->to_string() <<
				//	": " << ValueTypeName[constant->get_value()->type] << endl;
				return constant->get_value()->type;
			} else
			{
				// wcout << "anyval: " << to_string(symbol) << endl;
				return AnyVal;
			}
		};
	}

}
