#pragma once
#include "log.h"

#include "RuntimeStack.h"

namespace CldRuntime 
{
	struct BlockResult 
	{
		RuntimeStackSlot return_value;
		BlockResult()
		{
		}

		RuntimeValue * GetPtrToResult();
	};
}