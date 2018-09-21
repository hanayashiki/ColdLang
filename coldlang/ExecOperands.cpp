#include "stdafx.h"
#include "ExecOperands.h"
#include "RuntimeStack.h"

#include "Symbol.h"

#include "log.h"
#include "ExecDebug.h"

#define X86 asmjit::x86

using namespace CldRuntime;
using namespace asmjit;

namespace Compile {

	const vector<asmjit::X86Gp> ExecOperands::ParameterGps({
		X86::rcx, X86::rdx, X86::r8, X86::r9
	});

	const set<asmjit::X86Gp> ExecOperands::NonVolatileGps({
		X86::rbx, X86::rbp, X86::rdi, X86::rsi, X86::r12,
		X86::r13, X86::r14, X86::r15
	});

	const set<asmjit::X86Gp> ExecOperands::VolatileGps({
		X86::rax, X86::rcx, X86::rdx, X86::r8, X86::r9,
		X86::r10, X86::r11
	});

	const set<asmjit::X86Gp> ExecOperands::SaveOnCallGps({
		X86::rcx, X86::rdx, X86::r8, X86::r9
	});

	const set<asmjit::X86Gp> ExecOperands::InitialFreeGps({
		X86::rbx, X86::rdi, X86::rsi, X86::r12,
		X86::r13, X86::r14, X86::r15,
		X86::rax, /*X86::rcx,*/ X86::rdx, X86::r8, X86::r9,
	});

	ExecOperands::ExecOperands(
		const initializer_list<Variable*> & arguments,
		const initializer_list<X86Gp> & locations)
	{
		// FreeGps = InitialFreeGps;

		for (auto x : InitialFreeGps) 
		{
			FreeGps.emplace(x);
		}

		assert(arguments.size() == locations.size());

		CLD_DEBUG << "InitialFreeGps.size(): " << InitialFreeGps.size() << std::endl;
		CLD_DEBUG << "FreeGps.size(): " << FreeGps.size() << std::endl;

		auto argument = arguments.begin();
		auto location = locations.begin();
		for (;
			argument != arguments.end() &&
			location != locations.end();
			argument++, location++)
		{
			AllocVariable(*argument, *location);
		}

		CLD_DEBUG << DumpVariableToOperand() << std::endl;
	}

	ExecOperands::~ExecOperands()
	{
	}

	void ExecOperands::ReleaseReg(const X86Gp & gp)
	{
		UsedGps.erase(gp);
		FreeGps.emplace(gp);
	}

	void ExecOperands::AllocReg(const X86Gp & gp)
	{
		UsedGps.emplace(gp);
		FreeGps.erase(gp);
	}

	bool ExecOperands::IsFree(const X86Gp & gp)
	{
		return UsedGps.find(gp) == UsedGps.end();
	}

	X86Mem ExecOperands::GenerateMem(const Variable * var)
	{
		// TODO: closure ?
		X86Mem mem(runtimeStack, var->get_var_id() * 16, 16);
		return mem;
	}

	void ExecOperands::ReleaseVariable(const Variable * var)
	{
		Operand gp = VariableToOperand.at(var);
		assert(gp.isReg());
		VariableToOperand.erase(var);
		RegToVariable.erase(gp.as<X86Gp>());
		InRegVariables.erase(var);
		ReleaseReg(gp.as<X86Gp>());
	}

	const ExecOperands::Variable* ExecOperands::GetInRegVariable(const set<const Variable*> & excluding)
	{
		for (auto iter = InRegVariables.begin();
			iter != InRegVariables.end();
			iter++) 
		{
			if (excluding.find(*iter) == excluding.end()) 
			{
				return *iter;
			}
		}
		// Should not reach here
		assert(false);
	}

	const ExecOperands::Variable * ExecOperands::FindVariableByGp(const X86Gp & gp)
	{
		auto iter = RegToVariable.find(gp);
		if (iter != RegToVariable.end())
		{
			return iter->second;
		}
		else
		{
			return nullptr;
		}
	}

	bool ExecOperands::AllocVariable(const Variable * var, const X86Gp & gp)
	{
		if (!IsFree(gp)) {
			CLD_DEBUG << "gp not free" << std::endl;
			return false;
		}
		else {
			CLD_DEBUG << "alloc " << IR::OperandType::to_string(var) << std::endl;
			AllocReg(gp);
			VariableToOperand[var] = gp;
			RegToVariable[gp] = var;
			CLD_DEBUG << DumpVariableToOperand() << std::endl;
			InRegVariables.emplace(var);
			return true;
		}
	}

	const std::vector<X86Gp> ExecOperands::GetUsedSaveOnCallGps()
	{
		return std::vector<X86Gp>(
			UsedSaveOnCallGps.begin(),
			UsedSaveOnCallGps.end()
		);
	}

	const std::vector<X86Gp> ExecOperands::GetUsedVolatileGps()
	{
		return std::vector<X86Gp>(
			UsedVolatileGps.begin(), 
			UsedVolatileGps.end()
		);
	}

	const std::vector<X86Gp> ExecOperands::GetUsedNonVolatileGps()
	{
		return std::vector<X86Gp>(
			UsedNonVolatileGps.begin(),
			UsedNonVolatileGps.end()
		);
	}

	void ExecOperands::RecordGpUse(const X86Gp & gp)
	{
		RecordIfVolatile(gp);
		RecordIfSaveOnCallGps(gp);
		RecordIfNonVolatile(gp);
	}

	void ExecOperands::RecordIfVolatile(const X86Gp & gp)
	{
		if (VolatileGps.find(gp) != VolatileGps.end())
		{
			UsedVolatileGps.emplace(gp);
		}
	}

	void ExecOperands::RecordIfSaveOnCallGps(const X86Gp & gp)
	{
		if (SaveOnCallGps.find(gp) != SaveOnCallGps.end())
		{
			UsedSaveOnCallGps.emplace(gp);
		}
	}

	void ExecOperands::RecordIfNonVolatile(const X86Gp & gp)
	{
		if (NonVolatileGps.find(gp) != NonVolatileGps.end())
		{
			UsedNonVolatileGps.emplace(gp);
		}
	}

	void ExecOperands::ClearUsedVolatileGps()
	{
		UsedVolatileGps.clear();
	}

	void ExecOperands::ClearUsedSaveOnCallGps()
	{
		UsedSaveOnCallGps.clear();
	}

	wstring ExecOperands::to_string(const std::unordered_map<const Variable*, Operand>& v2o) const
	{
		using IR::OperandType::to_string;

		wstring str = L"{";
		for (auto pair : v2o) {
			const Variable * var = pair.first;
			Operand operand = pair.second;
			auto keyVal = to_string(var) + L": " + ExecDebug::to_string(operand)+L", ";
			str.append(keyVal);
		}
		str += L"}";
		return str;
	}

	bool ExecOperands::ToReg(const Variable * var)
	{
		CLD_DEBUG << "FreeGps.size() == " << FreeGps.size() << std::endl;
 		if (FreeGps.size() == 0) {
			return false;
		}
		const X86Gp & gp = FreeGps.begin()->clone();
		return AllocVariable(var, gp);
	}

	bool ExecOperands::ToMem(const Variable * var)
	{
		if (!InReg(var)) {
			return false;
		}
		ReleaseVariable(var);
		VariableToOperand[var] = GenerateMem(var);
		return true;
	}

	bool ExecOperands::InReg(const Variable * var)
	{
		auto find = VariableToOperand.find(var);
		return find != VariableToOperand.end() &&
			find->second.isReg();
	}

	bool ExecOperands::InMem(const Variable * var)
	{
		auto find = VariableToOperand.find(var);
		return find != VariableToOperand.end() &&
			find->second.isMem();
	}

	X86Mem ExecOperands::GetMem(const Variable * var)
	{
		assert(InMem(var));
		return VariableToOperand.find(var)->second.as<X86Mem>();
	}

	X86Gp ExecOperands::GetReg(const Variable * var)
	{
		assert(InReg(var));
		return VariableToOperand.find(var)->second.as<X86Gp>();
	}

}