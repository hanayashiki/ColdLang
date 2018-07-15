#pragma once
#include "stdafx.h"
#include "BytecodeWriter.h"

namespace IR
{
	class BytecodeDumper : public BytecodeWriter
	{
	public:
		virtual void emit(BytecodeClass::BytecodeBase& bytecode) override
		{
			// todo
			wcout << bytecode.to_string() << endl;
		}
	};
}
