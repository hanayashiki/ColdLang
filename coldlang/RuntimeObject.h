#pragma once
#include "stdafx.h"
#include "Function.h"


namespace Runtime 
{
	enum ValueType
	{
		IntegerVal = 1,
		FloatVal,
		PointerVal
	};

	enum ObjectType
	{
		StringObj = 1,
		FunctionObj
	};

	struct RuntimeValue
	{
		ValueType type;
	};

	struct IntegerValue : RuntimeValue
	{
		int64_t value;
	};

	struct FloatValue : RuntimeValue
	{
		double value;
	};

	struct RuntimeObject;
	struct PointerValue : RuntimeValue
	{
		RuntimeObject * value;
	};

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

	struct FunctionObject : RuntimeObject
	{
		void * func_ptr;
		IR::Function * meta;
	};
}
