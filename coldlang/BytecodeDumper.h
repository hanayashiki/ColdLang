#pragma once
#include "stdafx.h"
#include "BytecodeWriter.h"

namespace IR
{
	class BytecodeDumper : public BytecodeWriter
	{
	public:
		void emit(BytecodeClass::BytecodeBase & bytecode) override
		{
			// todo
			wcout << bytecode.to_string() << endl;
		}
		void bind(OperandType::Label & label) override
		{
			wcout << label.to_string() + L":" << endl;
		}
	};
}
