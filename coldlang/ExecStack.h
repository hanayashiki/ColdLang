#pragma once

#include "../asmjit/asmjit.h"

/*        Stack allocation (Win X64)
	++++++++++++++++++++++++++++++
	+                            +
    +        					 +
	+		in-mem parameters	 +------
    +							 +	   -
    +							 +	   -
-40	++++++++++++++++++++++++++++++	   -
	+                            +	   -
	+                            +	   -
	+       shadow space         +	   -- Aka `reserved for callee` (decribed below)
	+                            +	   -
	+                            +	   -
	+                            +	   -
 -8	++++++++++++++++++++++++++++++	   -
	+       return address       +------
  0	++++++++++++++++++++++++++++++
    +							 +
	+		 pushed regs		 +
	+							 +
	++++++++++++++++++++++++++++++
	+							 +
	+        saved regs			 +
	+							 +
	++++++++++++++++++++++++++++++
	+							 +
	+							 +
	+         temp mem			 +
	+							 +
	+							 +
	++++++++++++++++++++++++++++++ <---- [rbp]
	+                            +
	+     reserved for callee    +
	+                            +
	++++++++++++++++++++++++++++++ <---- [rsp] (16-byte aligned)

 */

#define DEF_INSERT_TASK(rbpBased, rspBased) \
	[=](const asmjit::X86Mem & rbpBased, const asmjit::X86Mem & rspBase) -> void

namespace Compile
{
	class ExecCompiler;

	class ExecStack
	{
	public:
		typedef asmjit::CBNode CBNode;
		typedef asmjit::Operand Operand;
		typedef asmjit::X86Gp X86Gp;
		typedef asmjit::X86Mem X86Mem;

		typedef std::function<void(const X86Mem &, const X86Mem &)> InsertTask;

		enum StackArea {
			SavedRegs = 1,
			TempMem = 2
		};
		struct StackMem {
			CBNode * placeHolder;
			StackArea area;
			uint32_t offset;
			uint32_t size;
		};
		struct StackTask {
			StackMem mem;
			InsertTask insertFunc;
		};

		StackMem Mem(CBNode * placeHolder, StackArea area, uint32_t offset, uint32_t size) { return StackMem{ placeHolder, area, offset, size }; }

		inline const static uint32_t ShadowRegMax = 4;
		inline const static uint32_t BaseCalleeStackSize = 32;

		explicit ExecStack(ExecCompiler *);
		void AllocCalleeStack(uint32_t size);
		void ReuseCalleeStack();

		/* Notify ExecStack to prepare to insert some mem-related instrs 
		   Note that InsertFunc accepts <X86Mem rbpBased, X86Mem rspBased> as input.
		 */
		void Preinsert(const StackMem & mem, InsertTask InsertFunc);
		void Insert(CBNode * prologPoint, CBNode * epilogPoint,
			const std::vector<X86Gp> & reserved);

		void ExecAllTasks();
		void ExecTask(const StackTask &);
	private:
		std::vector<StackTask> preinserts;

		uint32_t maxSavedRegsSize;
		uint32_t maxTempMemSize;

		uint32_t currentStackSize;
		uint32_t currentCalleeStackSize;
		uint32_t maxCalleeStackSize;
		ExecCompiler * exec;

	};
}