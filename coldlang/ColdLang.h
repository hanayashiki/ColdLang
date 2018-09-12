#pragma once

#include "RuntimeValue.h"

namespace Cld
{
	class Interpreter
	{
	public:
		static Interpreter * Instance(const wchar_t * code);
		virtual CldRuntime::RuntimeValue * Main() = 0;

		template<typename RetType>
		static RetType * RunCode(const wchar_t * code)
		{
			auto interpreter = std::unique_ptr<Cld::Interpreter>(Cld::Interpreter::Instance(code));
			CldRuntime::RuntimeValue * val = interpreter->Main();
			return reinterpret_cast<RetType*>(val);
		}
	};
}