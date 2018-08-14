#pragma once
#include "Symbol.h"
#include "RuntimeObject.h"
#include "BytecodeReader.h"
#include <functional>

namespace IR
{
	using namespace OperandType;
	using namespace BytecodeClass;
	using namespace CldRuntime;

	struct BinaryTypeInfo
	{
		ValueType target_type;
		Symbol * target;
		OpType op_type;
		ValueType left_type;
		Symbol * left_symbol;
		ValueType right_type;
		Symbol * right_symbol;
	};

	struct UnaryTypeInfo
	{
		ValueType target_type;
		Symbol * target;
		OpType op_type;
		ValueType source_type;
		Symbol * source;
	};

	struct JumpInfo
	{
		BytecodeEnum bytecode_enum;
		OperandType::Label::id_type label_id;
	};

	typedef JumpInfo LabelInfo;

	typedef unordered_map<Symbol*, ValueType>
		SymbolToType;

	class BytecodeTyper
	{
	private:
		static UnaryTypeInfo get_unary_info(function<ValueType(Symbol*)> get_type_of, unsigned char buf[]);
		static BinaryTypeInfo get_binary_info(function<ValueType(Symbol*)> get_type_of, unsigned char buf[]);
		static JumpInfo get_jump_info(const unsigned char buf[]);
		static function<ValueType(Symbol*)> get_get_type_of(SymbolToType & type_info);
	public:
		struct BytecodeTypeInfo
		{
			enum
			{
				Unary,
				Binary,
				Jump,
				Label,
				Unknown		
			} type;
			union
			{
				UnaryTypeInfo unary;
				BinaryTypeInfo binary;
				JumpInfo jump;
				LabelInfo label;
			};
		};
		static BytecodeTypeInfo get_bytecode_type_info(SymbolToType & type_info, BytecodeReader * reader);
		static BytecodeTypeInfo get_bytecode_type_info(SymbolToType & type_info, unsigned char buf[]);

	};
}
