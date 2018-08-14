#pragma once
#include "../asmjit/asmjit.h"
#include "BytecodeClass.h"
#include "Code.h"

namespace Compile
{
	using namespace asmjit;
	using namespace IR;
	using namespace BytecodeClass;
	using namespace CldRuntime;

	class Compiler
	{
	private:
		Compiler * next_ = nullptr;
	protected:
		static const bool executable = false;
		virtual void CompileBinaryImpl(Symbol * target, OpType op_type,
			ValueType left_type, Symbol * left_symbol,
			ValueType right_type, Symbol * right_symbol) = 0;
		virtual void CompileUnaryImpl(Symbol * target, OpType op_type,
			ValueType type, Symbol* left_symbol) = 0;
	public:
		virtual void CompileBinary(Symbol * target, OpType op_type, 
			ValueType left_type, Symbol * left_symbol, 
			ValueType right_type, Symbol * right_symbol)
		{
			CompileBinaryImpl(target, op_type, left_type, left_symbol,
				right_type, right_symbol);
			if (next_) {
				next_->CompileBinary(target, op_type, left_type, left_symbol,
					right_type, right_symbol);
			}
		}

		virtual void CompileUnary(Symbol * target, OpType op_type,
			ValueType type, Symbol* left_symbol)
		{
			CompileUnaryImpl(target, op_type, type, left_symbol);
			if (next_)
			{
				next_->CompileUnary(target, op_type, type, left_symbol);
			}
		}

		Compiler * PassTo(Compiler * next)
		{
			next_ = next;
			return this;
		}

		virtual Code GetCode() = 0;

		virtual ~Compiler()
		{
			delete next_;
		}
	};
}
