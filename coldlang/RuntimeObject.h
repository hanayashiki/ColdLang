#pragma once
#include "stdafx.h"
#include "Function.h"
#include "RuntimeValue.h"

#define VARIADIC(type, field_name) \
	inline type * field_name() \
	{ \
		return reinterpret_cast<type*>((size_t)this + sizeof(*this)); \
	}


namespace CldRuntime 
{

	struct RuntimeObject
	{
		ObjectType type;
		size_t ref;
		bool constant;
	};

	struct StringObject : RuntimeObject
	{
		size_t length;
		wchar_t * content;
	};

	struct RuntimeStack;
	struct FunctionObject : RuntimeObject
	{
		void * func_ptr;
		IR::Function * meta;
		VARIADIC(RuntimeStack, contexts)
	};
}
