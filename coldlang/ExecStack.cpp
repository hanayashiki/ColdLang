#include "stdafx.h"
#include "ExecStack.h"
#include "ExecCompiler.h"

#include "utils.h"

namespace Compile
{
	ExecStack::ExecStack(Compile::ExecCompiler * exec)
		: exec(exec),
		maxSavedRegsSize(0),
		maxTempMemSize(0),
		currentStackSize(0),
		currentCalleeStackSize(0),
		maxCalleeStackSize(currentCalleeStackSize)
	{

	}

	void ExecStack::AllocCalleeStack(uint32_t size)
	{
		if (currentCalleeStackSize == 0)
		{
			/* If this is called, it is indicated that 
			 * a sub-function will be called, 
			 * so shadowspace is allocated.
			 */
			currentCalleeStackSize += ExecStack::BaseCalleeStackSize;
		}
		currentCalleeStackSize += size;
		maxCalleeStackSize = std::max(currentCalleeStackSize, maxCalleeStackSize);
	}

	void ExecStack::ReuseCalleeStack()
	{
		currentCalleeStackSize = 0;
	}

	void ExecStack::Preinsert(const StackMem & mem, InsertTask InsertFunc)
	{
		switch (mem.area) {
		case SavedRegs:
			maxSavedRegsSize = max(maxSavedRegsSize, mem.offset + mem.size);
			break;
		case TempMem:
			maxTempMemSize = max(maxTempMemSize, mem.offset + mem.size);
			break;
		}
		currentStackSize = maxSavedRegsSize + maxTempMemSize;
		preinserts.push_back(StackTask{ mem, InsertFunc });
	}

	void ExecStack::Insert(CBNode * prologPoint, CBNode * epilogPoint,
		const std::vector<X86Gp> & reserved)
	{
		shared_ptr<X86Compiler> compiler = exec->compiler;

		/* At prolog */

		compiler->setCursor(prologPoint);
		exec->Comment("Prolog");
		int pushedRegCount;
		
		/* Save registers */

		std::vector<X86Gp> shadowedGps;
		std::vector<X86Gp> pushedGps;

		for (auto & gp : reserved) 
		{
			if (shadowedGps.size() < ShadowRegMax)
			{
				int id = shadowedGps.size();
				shadowedGps.push_back(gp);
				compiler->mov(x86::ptr_64(x86::rsp, id * 8 + 8), gp);
			}
			else 
			{
				compiler->push(gp);
				pushedGps.push_back(gp);
			}
		}

		/* if currentStack is built, we will modify rbp to point at the root */

		if (currentStackSize > 0)
		{
			compiler->push(x86::rbp);
			pushedGps.push_back(x86::rbp);
		}

		uint32_t realRspOffset = 0;
		if (currentStackSize + maxCalleeStackSize > 0)
		{
			uint32_t alignment = (pushedGps.size() + 1) % 2 == 0 ? 0 : 8; 
			// `1` for `call` has pushed addr
			realRspOffset = alignment + CldUtils::Align16(currentStackSize + maxCalleeStackSize);
			compiler->sub(x86::rsp, realRspOffset);
		}

		if (maxCalleeStackSize > 0)
		{
			exec->Comment("Set for maxCalleeStackSize");
			compiler->lea(x86::rbp, x86::ptr(x86::rsp, maxCalleeStackSize));
		}
		
		/* At epilog */

		compiler->setCursor(epilogPoint);
		exec->Comment("Epilog");
		if (currentStackSize + currentCalleeStackSize > 0)
		{
			compiler->lea(x86::rsp, x86::ptr(x86::rbp, realRspOffset - maxCalleeStackSize));
		}

		/* Restore regs */

		for (int i = pushedGps.size() - 1; i >= 0; i--)
		{
			compiler->pop(pushedGps[i]);
		}

		for (size_t i = 0; i < shadowedGps.size(); i++)
		{
			exec->Comment("Restore shadowed");
			compiler->mov(shadowedGps[i], x86::ptr_64(x86::rsp, i * 8 + 8));
		}

		/* Finish the tasks */

		ExecAllTasks();

	}

	void ExecStack::ExecAllTasks()
	{
		for (const auto & task : preinserts) 
		{
			ExecTask(task);
		}
	}

	void ExecStack::ExecTask(const StackTask & task)
	{
		shared_ptr<X86Compiler> compiler = exec->compiler;

		const StackMem & mem = task.mem;
		const auto & insertFunc = task.insertFunc;

		compiler->setCursor(mem.placeHolder);

		X86Mem rbpBased;
		X86Mem rspBased = x86::ptr(x86::rsp, mem.offset, mem.size);
		switch (mem.area)
		{
		case SavedRegs:
			rbpBased = x86::ptr(x86::rbp, mem.offset + maxTempMemSize, mem.size);
			break;
		case TempMem:
			rbpBased = x86::ptr(x86::rbp, mem.offset, mem.size);
			break;
		default:
			assert(false);
		}

		exec->Comment("stackManager inserts");

		insertFunc(rbpBased, rspBased);

		compiler->removeNode(mem.placeHolder);
	}

}