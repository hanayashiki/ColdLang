#include "stdafx.h"
#include "Code.h"
#include "ExecCompiler.h"

namespace Compile
{ 
	IR::Variable Code::RuntimeStackPtr = IR::Variable(Word::mock(L"%RuntimeStack"));
	IR::Variable Code::BlockResultPtr = IR::Variable(Word::mock(L"%BlockResultPtr"));
	IR::Variable Code::NContexts = IR::Variable(Word::mock(L"%NContexts"));
	IR::Variable Code::ContextsPtr = IR::Variable(Word::mock(L"%ContextsPtr"));

	Code::~Code()
	{
		ExecCompiler::jitRuntime.release(func_ptr_);
	}
}
