#pragma once
#include "BytecodeDef.h"

namespace IR
{
	namespace BytecodeClass
	{
#define ENUM_LINE_DEF(name, ...) Enum##name, 

	enum
	{
		BYTECODE_LIST(ENUM_LINE_DEF)
	};

#undef ENUM_LINE_DEF
	}
}
