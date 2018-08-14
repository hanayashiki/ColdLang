#pragma once
#include "Compiler.h"
#include "ExecCompiler.h"
#include "DumpingCompiler.h"

namespace Compile
{
	class CompilerFactory
	{
	public:
		static shared_ptr<Compiler> GetCompiler()
		{
			Compiler * exec = new ExecCompiler();
			Compiler * dump = new DumpingCompiler();
			exec->PassTo(dump);
			return shared_ptr<Compiler>(exec);
		}
	};
}
