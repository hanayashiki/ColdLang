#include "stdafx.h"
#include "Code.h"
#include "ExecCompiler.h"

namespace Compile
{ 
	Code::~Code()
	{
		ExecCompiler::jit_runtime_.release(func_ptr_);
	}
}
