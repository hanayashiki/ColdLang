#pragma once
#include "stdafx.h"
#include "RuntimeObject.h"
#include "NativeFunction.h"

namespace CldRuntime
{
	class NativeFunctionsDefs {
	private:
		typedef CldRuntime::ValueType ValueType;
		typedef IR::OperandType::NativeFunction NativeFunction;

		static std::vector<NativeFunction> native_functions;
	public:

		static std::vector<NativeFunction> & get_native_functions();

		/* Mapping type to signature */
		template<typename RetType, typename ...Args>
		static
		NativeFunction make_native(const wchar_t * name, RetType (*function)(Args...))
		{
			return NativeFunction(
				name, (void *)function, 
				std::initializer_list<ValueType>{ ValueType(Args::Signature)... }
			);
		}
	};
}