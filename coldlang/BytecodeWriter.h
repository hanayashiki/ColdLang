#pragma once
#include "BytecodeBase.h"

namespace IR
{
	class BytecodeWriter
	{
	public:
		virtual void emit(BytecodeClass::BytecodeBase &) = 0;
		virtual void bind(OperandType::Label &) = 0;
		virtual ~BytecodeWriter() = default;
	};
}