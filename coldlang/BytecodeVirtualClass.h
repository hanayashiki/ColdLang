#pragma once
#include "stdafx.h"
#include "BytecodeEnum.h"
#include "BytecodeBase.h"
#include "Symbol.h"
#include "Label.h"
#include "Literal.h"
#include "BytecodeLength.h"

namespace IR
{
	namespace BytecodeClass
	{
#define CLASS_DEF(name, acc_use, ...)\
	class Virtual##name : public BytecodeBase {\
	protected:\
		char byte_buf[100];\
		size_t byte_len = 0;\
	public:\
		virtual void init(__VA_ARGS__) = 0;\
		const char * get_name() {\
			return #name;\
		}\
		constexpr int get_id() {\
			static_assert(Enum##name <= 255 && Enum##name >= 0, "Too many bytecode types");\
			return Enum##name;\
		}\
		constexpr IR::AccumulatorUse::Type get_acc_use() {\
			return acc_use;\
		}\
		virtual wstring to_string() {\
			return tutils::ascii_string_to_wstring(get_name());\
		}\
		virtual void generate_byte(__VA_ARGS__) {\
		}\
		size_t dump_byte(char out_buf[]) {\
			memcpy(out_buf, byte_buf, byte_len);\
			return byte_len;\
		}\
	};

	BYTECODE_LIST(CLASS_DEF)
#undef CLASS_DEF
	}
}


