#pragma once
#include "RuntimeStack.h"

namespace CldRuntime 
{
	struct BlockResult 
	{
		RuntimeStackSlot return_value;
		BlockResult()
		{
		}

		inline RuntimeValue * GetPtrToResult(){ 
			return &(return_value.value.base_value);  
		}
	};
}