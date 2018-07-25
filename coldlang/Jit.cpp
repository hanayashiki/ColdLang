#include "stdafx.h"
#include "../asmjit/asmjit.h"
#include "RuntimeObject.h"
#include <stddef.h>
#include "native_functions.h"

namespace Compile
{
	using namespace IR;
	using namespace Runtime;
	using namespace IR::BytecodeClass;
	using namespace asmjit;

	Jit::~Jit()
	{
		delete compiler;
	}

	Jit::Jit(IR::BytecodeReader * bytecode_reader)
		:
		bytecode_reader_(bytecode_reader),
		logger(stderr)
	{

	}

	void Jit::Init()
	{
		code.init(jit_runtime.getCodeInfo());
		// code.setLogger(&logger);
		compiler = new X86Compiler(&code);
		compiler->addFunc(FuncSignature0<int>());
	}

	void Jit::CompileOne()
	{
		// temp for hello world
		unsigned char buf[100];
		size_t len;
		bytecode_reader_->read_byte(buf, &len);
		unsigned char code_id = buf[0];
		switch (code_id)
		{
		case EnumPushParamLiteral:
			CompilePushParamLiteral(buf);
			break;
		case EnumCallNativeFunc:
			CompileCallNative(buf);
			break;
		default:
			wcout << "CompileOne not implemented" << endl;
		}
	}

	void Jit::CompilePushParamLiteral(unsigned char buf[])
	{
		Literal * literal;
		memcpy(&literal, &buf[1], sizeof(literal));
		auto object = literal->get_object();

		// wcout << "RuntimeObj: " << object << endl;

		X86Mem stack = compiler->newStack(sizeof(PointerValue), STRUCT_ALIGN);
		X86Gp vRegPtrToStack = compiler->newGpq();
		compiler->lea(vRegPtrToStack, stack);
		compiler->mov(
			x86::ptr(vRegPtrToStack, offsetof(PointerValue, type), 4),
			imm(ValueType::PointerVal)
		);
		//compiler->mov(
		//	x86::ptr(vRegPtrToStack, offsetof(PointerValue, value), 8),
		//	imm(reinterpret_cast<size_t>(object))
		//);
		compiler->mov(
			x86::ptr(vRegPtrToStack, offsetof(PointerValue, value), 4),
			imm(reinterpret_cast<size_t>(object) & 0xFFFFFFFF)
		);
		compiler->mov(
			x86::ptr(vRegPtrToStack, offsetof(PointerValue, value) + 4, 4),
			imm((reinterpret_cast<size_t>(object) >> 32)  & 0xFFFFFFFF)
		);

		args.push_back(vRegPtrToStack);

	}

	void Jit::CompileCallNative(unsigned char buf[])
	{
		NativeFunction * native_func;
		memcpy(&native_func, &buf[1], sizeof(native_func));
		CCFuncCall * call =
			compiler->call(reinterpret_cast<uint64_t>(native_func->get_func_ptr()),
				FuncSignature1<int, void *>());
		// wcout << "native ptr: " << (long long int)(native_func->get_func_ptr()) << endl;
		// wcout << "native addr: " << (long long int)(&native_puts) << endl;

		call->setArg(0, args[0]);
		call->setRet(0, args[0]);
		args.clear();
	}

	Jit::Entrance Jit::GetEntrance()
	{
		Entrance entrance;
		compiler->ret();

		compiler->endFunc();                           // End of the function body.
		compiler->finalize();                          // Translate and assemble the whole `cc` content.
		jit_runtime.add(&entrance, &code);
		return entrance;
	}

}


