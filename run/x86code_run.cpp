#include "stdafx.h"

//#define DO_COMPILE

#ifdef DO_COMPILE
#include "../coldlang/ExecCompiler.h"
#include "../coldlang/ExecIntegerEmitter.h"
#include <functional>

using namespace Compile;

struct TestJitEnv {
	inline static JitRuntime jitRuntime;
	CodeHolder code;
	FileLogger logger;
	std::shared_ptr<X86Compiler> compiler;
	TestJitEnv()
		: logger(stdout)
	{
		code.init(jitRuntime.getCodeInfo());
		code.setLogger(&logger);
		compiler = std::make_shared<X86Compiler>(&code);
		compiler->addFunc(FuncSignature0<int32_t>());
	}

	function<uint64_t()> GetCode()
	{
		uint64_t (*func_ptr)();
		compiler->ret();

		compiler->endFunc();
		compiler->finalize();
		jitRuntime.add(&func_ptr, &code);
		return function([=]() { return func_ptr(); });
	}
};

// Reg Reg Reg
bool TestExecIntegerEmitter0() {
	TestJitEnv jitEnv;
	ExecIntegerEmitter emitter(jitEnv.compiler);
	jitEnv.compiler->mov(x86::rbx, 1);
	jitEnv.compiler->mov(x86::rcx, 2);
	emitter.Emit(x86::rax, OpAdd, x86::rbx, x86::rcx);
	jitEnv.compiler->ret(x86::rax);

	auto result = jitEnv.GetCode()();

	std::cout << result << std::endl;
	return result == (1 + 2);
}

// Reg Reg Mem
bool TestExecIntegerEmitter1() {
	TestJitEnv jitEnv;
	ExecIntegerEmitter emitter(jitEnv.compiler);
	uint64_t test;
	jitEnv.compiler->mov(x86::rbx, 1);
	jitEnv.compiler->mov(x86::rcx, (uint64_t)&test);
	jitEnv.compiler->mov(x86::qword_ptr(x86::rcx), 2);

	emitter.Emit(x86::rax, OpAdd, x86::rbx, x86::qword_ptr(x86::rcx));
	jitEnv.compiler->ret(x86::rax);

	auto result = jitEnv.GetCode()();

	std::cout << result << std::endl;
	return result == (1 + 2);
}

// Reg Mem Reg
bool TestExecIntegerEmitter2() {
	TestJitEnv jitEnv;
	ExecIntegerEmitter emitter(jitEnv.compiler);
	uint64_t test;
	jitEnv.compiler->mov(x86::rcx, (uint64_t)&test);
	jitEnv.compiler->mov(x86::qword_ptr(x86::rcx), 1);
	jitEnv.compiler->mov(x86::rbx, 2);

	emitter.Emit(x86::rax, OpAdd, x86::qword_ptr(x86::rcx), x86::rbx);
	jitEnv.compiler->ret(x86::rax);

	auto result = jitEnv.GetCode()();

	std::cout << result << std::endl;
	return result == (1 + 2);
}

// Reg Mem Mem
bool TestExecIntegerEmitter3() {
	TestJitEnv jitEnv;
	ExecIntegerEmitter emitter(jitEnv.compiler);
	uint64_t test1;
	uint64_t test2;
	jitEnv.compiler->mov(x86::rbx, (uint64_t)&test1);
	jitEnv.compiler->mov(x86::qword_ptr(x86::rbx), 1);
	jitEnv.compiler->mov(x86::rcx, (uint64_t)&test2);
	jitEnv.compiler->mov(x86::qword_ptr(x86::rcx), 2);

	emitter.Emit(x86::rax, OpAdd, x86::qword_ptr(x86::rcx), x86::qword_ptr(x86::rbx));
	jitEnv.compiler->ret(x86::rax);

	auto result = jitEnv.GetCode()();

	std::cout << result << std::endl;
	return result == (1 + 2);
}

// Mem Reg Reg
bool TestExecIntegerEmitter4() {
	TestJitEnv jitEnv;
	ExecIntegerEmitter emitter(jitEnv.compiler);
	uint64_t test;

	jitEnv.compiler->mov(x86::rax, (uint64_t)&test);
	jitEnv.compiler->mov(x86::rbx, 1);
	jitEnv.compiler->mov(x86::rcx, 2);

	emitter.Emit(x86::qword_ptr(x86::rax), OpAdd, x86::rbx, x86::rcx);
	jitEnv.compiler->mov(x86::rax, x86::qword_ptr(x86::rax));
	jitEnv.compiler->ret(x86::rax);

	auto result = jitEnv.GetCode()();

	std::cout << result << std::endl;
	return result == (1 + 2);
}


int main() 
{
	assert(
		TestExecIntegerEmitter0() &&
		TestExecIntegerEmitter1() &&
		TestExecIntegerEmitter2() &&
		TestExecIntegerEmitter3() &&
		TestExecIntegerEmitter4()
	);

	getchar();
	return 0;
}

#endif