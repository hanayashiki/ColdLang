#include "stdafx.h"

#include "BlockResult.h"
#include "log.h"

namespace CldRuntime
{
	RuntimeValue * BlockResult::GetPtrToResult()
	{
		std::wcerr << "this = " << this << std::endl;
		CLD_DEBUG << LOG_EXPR(this) << std::endl;
		CLD_DEBUG << LOG_EXPR(&return_value) << std::endl;
		CLD_DEBUG << LOG_EXPR(&return_value.value) << std::endl;
		CLD_DEBUG << LOG_EXPR(&return_value.value.base_value) << std::endl;
		CLD_DEBUG << LOG_EXPR(&(return_value.value.base_value)) << std::endl;
		return &(return_value.value.base_value);
	}
}