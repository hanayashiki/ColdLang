#include "stdafx.h"
#include "BytecodeTyper.h"
#include <functional>
#include "TypeCalc.h"
#include "MiscellaneousBytecodeInfoGetters.h"
#include "BytecodeClassHelper.h"

#include "log.h"

namespace IR
{
	BytecodeTyper::BytecodeTypeInfo IR::BytecodeTyper::get_bytecode_type_info(SymbolToType & type_info, BytecodeReader * reader)
	{
		unsigned char buf[128];
		size_t read_count;
		reader->read_byte(buf, &read_count);

		assert(read_count > 0);
		return get_bytecode_type_info(type_info, buf);
	}

	BytecodeTyper::BytecodeTypeInfo BytecodeTyper::get_bytecode_type_info(SymbolToType & type_info, unsigned char buf[])
	{
		function<ValueType(Symbol*)> get_type_info = get_get_type_of(type_info);

		switch (buf[0])
		{
		case EnumPushParamVariable:
		case EnumPushParamLiteral:
		case EnumCallNative:
		case EnumRetAcc:
		case EnumInc:
		case EnumDecre:
		{
			SingleTypeInfo single_type_info = get_single_type_info(get_type_info, buf);
			BytecodeTypeInfo type_info;
			type_info.type = BytecodeTypeInfo::Single;
			type_info.single = single_type_info;
			return type_info;
			break;
		}
		case EnumJump:
		case EnumJumpOnFalse:
		case EnumJumpOnTrue:
		case EnumLabel:
		{
			JumpInfo jump_info = get_jump_info(buf);
			BytecodeTypeInfo type_info;
			if (is_jump(buf)) {
				type_info.type = BytecodeTypeInfo::Jump;
				type_info.jump = jump_info;
			} else if (is_label(buf))
			{
				type_info.type = BytecodeTypeInfo::Label;
				type_info.label = jump_info;
			} else
			{
				assert(false);
			}
			return type_info;
			break;
		}
		case EnumStoreAcc:
		case EnumLoadToAccLiteral:
		case EnumLoadToAccVariable:
		{
			// wcout << "load/store" << endl;
			UnaryTypeInfo unary_info = get_unary_info(get_type_info, buf);
			BytecodeTypeInfo type_info;
			type_info.type = BytecodeTypeInfo::Unary;
			type_info.unary = unary_info;
			return type_info;
			break;
		}
		default:
			// wcout << "buf[0]" << hex << (int)(buf[0]) << dec << endl;
			if (is_unary(buf))
			{
				BinaryTypeInfo binary_info = get_binary_info(get_type_info, buf);
				BytecodeTypeInfo type_info;
				type_info.type = BytecodeTypeInfo::Binary;
				type_info.binary = binary_info;
				return type_info;
			}
			else
			{
				wcout << "Not implemented" << endl;
				return
				{
					BytecodeTypeInfo::Unknown,
					UnaryTypeInfo()
				};
			}
		}

	}

	Symbol * BytecodeTyper::retrieve_on_type(
		BytecodeEnum this_enum,
		BytecodeEnum enum_var,
		BytecodeEnum enum_literal, const unsigned char buf[])
	{
		if (this_enum == enum_var)
		{
			return retrieve_arg<OperandType::Variable*>(&buf[1]);;
		}
		else if (this_enum == enum_literal)
		{
			return retrieve_arg<OperandType::Literal*>(&buf[1]);;
		}
		else {
			assert(false);
			return nullptr;
		}

	}

	UnaryTypeInfo BytecodeTyper::get_unary_info(function<ValueType(Symbol*)> get_type_of, unsigned char buf[])
	{
		UnaryTypeInfo unary_type_info;
		UnaryBytecodeInfo bytecode_info;

		switch (buf[0])
		{
		case EnumStoreAcc:
		{
			bytecode_info = get_unary_info_StoreAcc(buf);
			unary_type_info.op_type = OpMove;
			unary_type_info.source_type = get_type_of(Symbol::Acc);
			unary_type_info.source = Symbol::Acc;
			unary_type_info.target_type = get_type_of(Symbol::Acc);
			unary_type_info.target = bytecode_info.symbol;
			return unary_type_info;
			break;
		}
		case EnumLoadToAccVariable:
		case EnumLoadToAccLiteral:
		{
			GetUnaryInfo unary_info_getter = get_get_unary_info(buf);
			bytecode_info = unary_info_getter(buf);
			unary_type_info.op_type = OpMove;
			unary_type_info.source_type = get_type_of(bytecode_info.symbol);
			unary_type_info.source = bytecode_info.symbol;
			unary_type_info.target_type = get_type_of(bytecode_info.symbol);
			unary_type_info.target = Symbol::Acc;
			return unary_type_info;
			break;
		}
		default:
			wcout << "Not implemented" << endl;
			assert(false);
		}
	}

	BinaryTypeInfo BytecodeTyper::get_binary_info(function<ValueType(Symbol*)> get_type_of, unsigned char buf[])
	{
		BinaryTypeInfo binary_type_info;
		GetUnaryInfo get_unary_info = get_get_unary_info(buf);
		UnaryBytecodeInfo unary_info = get_unary_info(buf);

		binary_type_info.target = Symbol::Acc;
		binary_type_info.left_symbol = Symbol::Acc;
		binary_type_info.right_symbol = unary_info.symbol;

		binary_type_info.op_type = unary_info.op;
		binary_type_info.left_type = get_type_of(binary_type_info.left_symbol);
		binary_type_info.right_type = get_type_of(binary_type_info.right_symbol);
		binary_type_info.target_type = TypeCalc::get_result_type(unary_info.op,
			binary_type_info.left_type, binary_type_info.right_type);

		return binary_type_info;
	}

	JumpInfo BytecodeTyper::get_jump_info(const unsigned char buf[])
	{
		assert(is_jump(buf) || is_label(buf));
		JumpInfo jump_info;
		jump_info.bytecode_enum = BytecodeEnum(buf[0]);
		jump_info.label_id = retrieve_arg<OperandType::Label*>(&buf[1])->get_id();
		return jump_info;
	}

	SingleTypeInfo BytecodeTyper::get_single_type_info(function<ValueType(Symbol*)> get_type_of, const unsigned char buf[])
	{
		SingleTypeInfo single_type_info;
		single_type_info.bytecode_enum = BytecodeEnum(buf[0]);
		switch (buf[0]) {
		case EnumPushParamVariable:
		case EnumPushParamLiteral:
			{
				Symbol * symbol = retrieve_on_type(single_type_info.bytecode_enum,
					EnumPushParamVariable, EnumPushParamLiteral, buf);
				single_type_info.result_type = NoneVal;
				single_type_info.origin_type = get_type_of(symbol);
				single_type_info.target = symbol;
				single_type_info.write_acc = false;
				break;
			}
		case EnumCallNative:
			{
				NativeFunction* symbol = retrieve_arg<OperandType::NativeFunction*>(&buf[1]);
				single_type_info.result_type = symbol->get_ret_type();
				single_type_info.origin_type = NoneVal;
				single_type_info.target = symbol;
				single_type_info.write_acc = true;
				break;
			}
		case EnumRetAcc:
			{
				single_type_info.result_type = get_type_of(Symbol::Acc);
				single_type_info.origin_type = get_type_of(Symbol::Acc);
				single_type_info.target = Symbol::Acc;
				single_type_info.write_acc = false;
				break;
			}
		case EnumInc:
		case EnumDecre:
			{
				Symbol* symbol = retrieve_arg<OperandType::Variable*>(&buf[1]);
				single_type_info.result_type = get_type_of(symbol);
				single_type_info.origin_type = get_type_of(symbol);
				single_type_info.target = symbol;
				single_type_info.write_acc = false;
				break;
			}
		}

		return single_type_info;
		
	}

	function<ValueType(Symbol*)> BytecodeTyper::get_get_type_of(SymbolToType & symbol_to_type)
	{
		return [&](Symbol* symbol) -> ValueType
		{
			if (symbol_to_type.find(symbol) != symbol_to_type.end())
			{
				//wcout << "get_get_type_of: symbol to type: " << &symbol_to_type << endl;
				return symbol_to_type.find(symbol)->second;
			}
			else if (auto literal = dynamic_cast<Literal*>(symbol))
			{
				CLD_DEBUG << "constant: " << literal->to_string() <<
					": " << ValueTypeName[literal->get_value()->type] << endl;
				return literal->get_value()->type;
			}
			else
			{
				// wcout << "anyval: " << to_string(symbol) << endl;
				return AnyVal;
			}
		};
	}
}
