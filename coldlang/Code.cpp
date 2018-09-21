#include "stdafx.h"
#include "Code.h"
#include "ExecCompiler.h"

#include "RuntimeValue.h"

namespace Compile
{ 
	IR::Variable Code::RuntimeStackPtr = IR::Variable(Word::mock(L"%RuntimeStack"), CldRuntime::IntegerVal, true);
	IR::Variable Code::BlockResultPtr = IR::Variable(Word::mock(L"%BlockResultPtr"), CldRuntime::IntegerVal, true);
	IR::Variable Code::NContexts = IR::Variable(Word::mock(L"%NContexts"), CldRuntime::IntegerVal, true);
	IR::Variable Code::ContextsPtr = IR::Variable(Word::mock(L"%ContextsPtr"), CldRuntime::IntegerVal, true);

	Code::~Code()
	{
		ExecCompiler::jitRuntime.release(func_ptr_);
	}
}
