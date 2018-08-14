#include "stdafx.h"
#include "ExecCompiler.h"
#include "ValueTypeVec.h"
#include "Code.h"

namespace Compile {
	JitRuntime ExecCompiler::jit_runtime_;

	ExecCompiler::ExecCompiler()
		: logger(stderr), debug(this)
	{
		code.init(jit_runtime_.getCodeInfo());
		code.setLogger(&logger);
		compiler = std::make_shared<X86Compiler>(&code);
		compiler->addFunc(FuncSignature0<CldRuntime::ValueTypeVec>());
	}

	X86Gp & ExecCompiler::GetTempReg64()
	{
		static int reg_id = 0;
		if (temp_reg64.size() == 0)
		{
			temp_reg64.push_back(compiler->newInt64("temp%d", reg_id));
		} else
		{
			X86Gp & gp = *temp_reg64.begin();
			temp_reg64.pop_front();
			return gp;
		}
	}

	void ExecCompiler::RevertTempReg64(const X86Gp & gp)
	{
		temp_reg64.push_front(gp);
	}


	X86Gp & ExecCompiler::IntegerReg(Variable * symbol)
	{
		/* TODO: impl for closure */
		if (variable_to_reg.find(symbol) == variable_to_reg.end())
		{
			char buf[128];
			std::wcstombs(buf, to_string(symbol).c_str(), sizeof(buf));
			variable_to_reg.insert(std::make_pair(symbol, compiler->newInt64(buf)));
		}
		return variable_to_reg[symbol];
	}

	void ExecCompiler::CompileBinaryImpl(Symbol * target, OpType op_type,
		ValueType left_type, Symbol * left_symbol, ValueType right_type, Symbol * right_symbol)
	{
		#define ADD_CODE(lt, rt, block) \
			if (left_type == (lt) && right_type == (rt)) { block }

		switch (op_type)
		{
		case OpAdd:
			ADD_CODE(IntegerVal, IntegerVal, 
			{
				AddCodeIntegerTwoOperands(target, op_type, left_symbol, right_symbol);
				
			})
			break;
		default:
			wcout << "Not implemented" << endl;
			assert(false);
		}

		#undef ADD_CODE
	}

	void ExecCompiler::CompileUnaryImpl(Symbol * target, OpType op_type, 
		ValueType type, Symbol * source)
	{
		#define ADD_CODE(source_type, block)\
			if (type == source_type) { block }

		switch (op_type)
		{
		case OpMove:
			ADD_CODE(IntegerVal, {
				AddCodeIntegerOneOperand(target, op_type, source);
			})
			break;
		default:
			wcout << "Not implemented" << endl;
			assert(false);
		}


		#undef ADD_CODE
	}

	void ExecCompiler::AddCodeIntegerTwoOperands(Symbol * target, OpType op_type, Symbol * left_symbol, Symbol * right_symbol)
	{
		auto target_var = dynamic_cast<Variable*>(target);
		assert(target != nullptr);
		auto left_var = dynamic_cast<Variable*>(left_symbol);
		auto right_var = dynamic_cast<Variable*>(right_symbol);
		auto left_const = dynamic_cast<Constant*>(left_symbol);
		assert(left_const == nullptr);
		auto right_const = dynamic_cast<Constant*>(right_symbol);
		if (left_var && right_var)
		{
			if (target == left_var)
			{
				AddCodeIntegerRegReg(op_type, IntegerReg(target_var), IntegerReg(right_var));
			} else
			{
				compiler->mov(IntegerReg(target_var), IntegerReg(left_var));
				AddCodeIntegerRegReg(op_type, IntegerReg(target_var), IntegerReg(right_var));
			}
		} else if (left_var && right_const)
		{
			RuntimeValue * runtime_value = right_const->get_value();
			assert(runtime_value->type == IntegerVal);
			int64_t integer_value = reinterpret_cast<IntegerValue*>(runtime_value)->value;
			if (target == left_var)
			{
				AddCodeIntegerRegImm(op_type, IntegerReg(target_var), imm(integer_value));
			} else
			{
				compiler->mov(IntegerReg(target_var), IntegerReg(left_var));
				AddCodeIntegerRegImm(op_type, IntegerReg(target_var), imm(integer_value));
			}
		}
		else {
			assert(false);
			// todo : more types
		}

		debug.PrintReg(target->to_string() + L": ", IntegerReg(target_var));
	}

	void ExecCompiler::AddCodeIntegerRegReg(OpType op_type, X86Gp & reg1, X86Gp & reg2) const
	{
		switch(op_type)
		{
		case OpAdd:
			compiler->add(reg1, reg2);
			break;
		case OpMove:
			compiler->mov(reg1, reg2);
			break;
		default:
			wcout << "Not implemented" << endl;
			assert(false);
		}
	}

	void ExecCompiler::AddCodeIntegerRegImm(OpType op_type, X86Gp & reg, Imm && imm) const
	{
		switch (op_type)
		{
		case OpAdd:
			compiler->add(reg, imm);
			break;
		case OpMove:
			compiler->mov(reg, imm);
			break;
		default:
			wcout << "Not implemented" << endl;
			assert(false);
		}
	}

	void Compile::ExecCompiler::AddCodeIntegerOneOperand(Symbol * target, OpType op_type, Symbol * source)
	{
		Variable * target_var = dynamic_cast<Variable*>(target);
		assert(target_var);
		Variable * source_var = dynamic_cast<Variable*>(source);
		Constant * source_constant = dynamic_cast<Constant*>(source);
		switch(op_type)
		{
		case OpMove:
			if (source_var)
			{
				AddCodeIntegerRegReg(op_type, IntegerReg(target_var), IntegerReg(source_var));
			} else
			{
				RuntimeValue * runtime_value = source_constant->get_value();
				int64_t integer_value = reinterpret_cast<IntegerValue*>(runtime_value)->value;
				AddCodeIntegerRegImm(op_type, IntegerReg(target_var), imm(integer_value));
			}
			break;
		default:
			wcout << "Not implemented" << endl;
			assert(false);
		}
		debug.PrintReg(target->to_string() + L": ", IntegerReg(target_var));

	}

	Code ExecCompiler::GetCode()
	{
		void * func_ptr;
		compiler->ret();

		compiler->endFunc();
		compiler->finalize();
		jit_runtime_.add(&func_ptr, &code);
		return Code(func_ptr);
	}
}
