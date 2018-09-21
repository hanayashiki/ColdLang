#pragma once
#include "Compiler.h"
#include "../asmjit/asmjit.h"
#include "Code.h"
#include "ExecDebug.h"
#include "ExecOperands.h"
#include "ExecIntegerEmitter.h"
#include "ExecStack.h"

#include "BytecodeTyper.h"

namespace Compile
{

	class ExecCompiler : public Compiler
	{
	public:
		ExecCompiler(IR::SymbolToType * symbolToType);
		virtual Code GetCode() override;
	private:
		typedef asmjit::X86Compiler X86Compiler;
		typedef asmjit::Operand Operand;
		typedef asmjit::X86Gp X86Gp;
		typedef asmjit::X86Mem X86Mem;

		struct OperandParams {
			Symbol * symbol;
			bool loadValue;
			ValueType valueType;
			bool operator< (const OperandParams & rhs) const {
				return symbol < rhs.symbol;
			}
		};

		struct Argument {
			Symbol * symbol = nullptr;
			bool isRef = false;
		};

		IR::SymbolToType * symbolToType;
		vector<Argument> arguments;

		friend class Code;
		friend class ExecDebug;
		friend class ExecStack;

		std::unique_ptr<ExecIntegerEmitter> integerEmitter;
		std::unique_ptr<ExecStack> stackManager;

		static JitRuntime jitRuntime;

		CodeHolder code;
		FileLogger logger;
		shared_ptr<X86Compiler> compiler;

		CBNode * DummyNode();

		CBNode * prologInsertingPoint;
		CBNode * epilogInsertingPoint;

		ExecDebug debug;
		ExecOperands operands;
		X86Gp & Def(X86Gp &);
		const X86Gp & Def(const X86Gp &);

		static const size_t OpTypeCount = sizeof(OpTypeName) / sizeof(OpTypeName[0]);

		typedef function<bool(ExecCompiler * _this, Symbol * target, Symbol* left, Symbol * right)> QuadHandler;
		inline static QuadHandler QuadHandlers[OpTypeCount][ValueTypeCount][ValueTypeCount];
		
		static bool isHandlersInited;
		static void InitQuadHandlers();

		vector<asmjit::Operand> Locate(const initializer_list<OperandParams> & symbols);
		asmjit::Operand Locate(Variable * var, bool loadVal, ValueType type, const set<const Variable*> & varGroup);
		asmjit::Operand Locate(Literal * constant);

		const X86Gp Reserve(const Variable * var);
		void Shelter(const X86Gp & gp); // If gp contains a var, reserve the var to stack

		vector<asmjit::Operand> LocateQuad(Symbol * target, ValueType type, Symbol * left, Symbol * right);

		void MoveMemToReg(X86Gp reg, X86Mem base, ValueType valueType);
		void MoveRegToMem(X86Mem base, X86Gp reg, ValueType valueType);
		void MemSync(const Variable * var);
		/* Uses rcx, rdi, rsi*/
		void MemMovD(X86Mem dst, X86Mem src, uint32_t sizeDword);

		void Ret(Symbol * target, ValueType sourceType);
		void CallNative(Symbol * target, ValueType sourceType);

		/* https://docs.microsoft.com/zh-cn/cpp/build/parameter-passing */
		void Push(Symbol * symbol);
		void PushParams(const vector<Argument> & args);
		uint32_t PushParam(const Argument & arg, uint32_t tempMemOffset, std::function<void(X86Mem)> ptrLoader);
		uint32_t PushParamToReg(const Argument & arg, uint32_t tempMemOffset, const X86Gp & gp);
		uint32_t PushParamToMem(const Argument & arg, uint32_t tempMemOffset, const X86Mem & mem);
		void Invoke(void * funcPtr);

		ValueType GetType(const Symbol* symbol);

		X86Mem WithOffsetAndSize(X86Mem mem, int64_t offset, uint32_t size);

		template<typename ...Args>
		inline void Comment(const char * fmt, Args... args)
		{
			char buf[256];
			int nHead = snprintf(buf, sizeof(buf), "; ");
			snprintf(buf + nHead, sizeof(buf) - nHead, fmt, args...);
			compiler->comment(buf);
		}

		template<typename ...Args>
		inline void Comment(std::string str, Args... args)
		{
			return Comment(str.c_str(), args...);
		}

	protected:
		static const bool executable = true;
		virtual void CompileBinaryImpl(Symbol * target, OpType opType,
			ValueType leftType, Symbol * leftSymbol,
			ValueType rightType, Symbol * rightSymbol) override;
		virtual void CompileUnaryImpl(Symbol * target, OpType opType,
			ValueType type, Symbol* source) override;
		virtual void CompileSingleImpl(Symbol * target, BytecodeEnum bytecodeName,
			ValueType sourceType) override;
	};
}
