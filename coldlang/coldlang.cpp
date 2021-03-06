// coldlang.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ColdLang.h"
#include "Intepreter.h"

namespace Cld {
	Interpreter * Cld::Interpreter::Instance(const wchar_t * code)
	{
		return new _Interpreter(code);
	}
}
