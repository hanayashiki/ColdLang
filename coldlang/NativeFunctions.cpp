#include "stdafx.h"
#include "NativeFunctions.h"
#include "RuntimeObject.h"

namespace CldRuntime
{
	void native_puts(PointerValue pointer)
	{
		assert(pointer.type == StringPointerVal);
		RuntimeObject * runtime_obj = pointer.value;
		assert(runtime_obj->type == StringObj);
		StringObject * string_object = reinterpret_cast<StringObject*>(runtime_obj);
		wcout << string_object->content;
	}
}