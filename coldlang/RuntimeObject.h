#pragma once
#include "stdafx.h"
#include "Function.h"


namespace CldRuntime 
{
	enum ValueType
	{
		AnyVal,
		IntegerVal,
		FloatVal,
		BooleanVal,
		NoneVal,
		StringPointerVal,
		FunctionPointerVal,
		ObjectPointerVal,
		ErrorVal
	};

	static const ValueType ValueVecEnd = ErrorVal;

	static const char * ValueTypeName[] = {
		"any",
		"int",
		"float",
		"bool",
		"none",
		"string_ptr",
		"func_ptr",
		"obj_ptr",
		"err_ptr"
	};

	enum ObjectType
	{
		StringObj = 1,
		FunctionObj
	};

	struct RuntimeValue
	{
		ValueType type;
		RuntimeValue(ValueType type) : type(type)
		{	
		}
	};

	struct IntegerValue : RuntimeValue
	{
		int64_t value;
		IntegerValue(int64_t value) : value(value), RuntimeValue(IntegerVal)
		{
		}
	};

	struct FloatValue : RuntimeValue
	{
		double value;
		FloatValue(double value) : value(value), RuntimeValue(FloatVal)
		{
		}
	};

	struct BooleanValue : RuntimeValue
	{
		bool value;
		BooleanValue(bool value) : value(value), RuntimeValue(BooleanVal)
		{
		}
	};

	struct NoneValue : RuntimeValue
	{
		NoneValue() : RuntimeValue(NoneVal)
		{
		}
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
