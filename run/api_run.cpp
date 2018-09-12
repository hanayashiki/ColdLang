#include "stdafx.h"
#include <iostream>
#include <memory>

#include "../coldlang/ColdLang.h"

#define DO_COMPILE
#ifdef DO_COMPILE

int main() 
{
	const wchar_t * code = LR"(
		r1 = 1
		r2 = 1
		r3 = 1
		r4 = 1
		r5 = 1
		r6 = 1
		r7 = 1
		r8 = 1
		r9 = 1
		r10 = 1
		r11 = 1
		r12 = 1
		r13 = 1
		r14 = 1
		r15 = 1
		c = r1 + r2 + r3 + r4 + r5 + r6 + r7 +
			r8 + r9 + r10 + r11 + r12 + r13 + r14 +
			r15
		retv c
	)";

	auto val = Cld::Interpreter::RunCode<CldRuntime::IntegerValue>(code);

	std::wcout << "result: " << val->value;

	getchar();
}

#endif