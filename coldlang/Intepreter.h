#pragma once

#include "ColdLang.h"
#include "RuntimeValue.h"
#include <string>

namespace Cld 
{
	class _Interpreter : public Interpreter
	{
	private:
		wstring code;
	public:
		_Interpreter(const wchar_t * code);
		virtual CldRuntime::RuntimeValue * Main();
	};
}