#include "stdafx.h"
#include "native_functions.h"
#include "RuntimeObject.h"

namespace Runtime
{
	void native_puts(PointerValue pointer)
	{
		assert(pointer.type == PointerVal);
		RuntimeObject * runtime_obj = pointer.value;
		assert(runtime_obj->type == StringObj);
		StringObject * string_object = reinterpret_cast<StringObject*>(runtime_obj);
		wcout << string_object->content;
	}
}