#pragma once
#include "stdafx.h"
#include "BytecodeReader.h"
#include "../asmjit/asmjit.h"
#include <iostream>

namespace Compile {
	using namespace asmjit;
	/* obsolete */
	class Jit
	{
	private:
		JitRuntime jit_runtime_;
		CodeHolder code;
		FileLogger logger;
		X86Compiler * compiler;

		IR::BytecodeReader* bytecode_reader_;
		const uint32_t STRUCT_ALIGN = 16;

		vector<X86Gp> args;
	public:
		Jit(IR::BytecodeReader* bytecode_reader);
		void Init();
		~Jit();

		typedef int(*Entrance)();
		void CompileOne();
		void CompilePushParamLiteral(unsigned char buf[]);
		void CompileCallNative(unsigned char buf[]);
		Entrance GetEntrance();
	};
}