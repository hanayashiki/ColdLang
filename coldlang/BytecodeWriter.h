#pragma once
#include "BytecodeBase.h"

namespace IR
{
	class BytecodeWriter
	{
	public:
		virtual void emit(BytecodeClass::BytecodeBase &) = 0; 
	};
}