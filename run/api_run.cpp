#include "stdafx.h"
#include "../coldlang/log.h"
#include <iostream>
#include <memory>

#include "../coldlang/ColdLang.h"

#define DO_COMPILE
#ifdef DO_COMPILE

int main() 
{
	CldLog::SetFilterLevel(CldLog::FilterLevel::Verbose);

	const wchar_t * code = LR"(
				c = 1
				d = 2
				e = c + d
				native_puts('Fuck you leather man!\n')
				native_puts('Son of a bitch!\n')
				retv e
	)";

	auto val = Cld::Interpreter::RunCode<CldRuntime::IntegerValue>(code);

	std::wcout << "result: " << val->value;

	getchar();
}

#endif