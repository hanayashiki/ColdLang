#pragma once
#include "Compiler.h"
#include "../asmjit/asmjit.h"
#include "Code.h"
#include "ExecDebug.h"
#include "ExecOperands.h"
#include "ExecIntegerEmitter.h"

#include "BytecodeTyper.h"

namespace Compile
{

	class ExecCompiler : public Compiler
	{
	public:
		ExecCompiler(IR::SymbolToType * symbolToType);
		virtual Code GetCode() override;
	private:
		struct OperandParams {
			Symbol * symbol;
			bool loadValue;
			ValueType valueType;
			bool operator< (const OperandParams & rhs) const {
				return symbol < rhs.symbol;
			}
		};
		IR::SymbolToType * symbolToType;

		friend class Code;
		friend class ExecDebug;

		std::unique_ptr<ExecIntegerEmitter> integerEmitter;

		static JitRuntime jitRuntime;

		CodeHolder code;
		FileLogger logger;
		shared_ptr<X86Compiler> compiler;

		ExecDebug debug;
		ExecOperands operands;

		static const size_t OpTypeCount = sizeof(OpTypeName) / sizeof(OpTypeName[0]);

		typedef function<bool(ExecCompiler * _this, Symbol * target, Symbol* left, Symbol * right)> QuadHandler;
		inline static QuadHandler QuadHandlers[OpTypeCount][ValueTypeCount][ValueTypeCount];
		
		static bool isHandlersInited;
		static void InitQuadHandlers();

		vector<asmjit::Operand> Locate(const initializer_list<OperandParams> & symbols);
		asmjit::Operand Locate(Variable * var, bool loadVal, ValueType type, const set<const Variable*> & varGroup);
		asmjit::Operand Locate(Constant * constant);

		vector<asmjit::Operand> LocateQuad(Symbol * target, ValueType type, Symbol * left, Symbol * right);

		void MoveMemToReg(X86Gp reg, X86Mem base, ValueType valueType);
		void MoveRegToMem(X86Mem base, X86Gp reg, ValueType valueType);

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

	protected:
		static const bool executable = true;
		virtual void CompileBinaryImpl(Symbol * target, OpType opType,
			ValueType leftType, Symbol * leftSymbol,
			ValueType rightType, Symbol * rightSymbol) override;
		virtual void CompileUnaryImpl(Symbol * target, OpType op_type,
			ValueType type, Symbol* source) override;
		virtual void CompileSingleImpl(Symbol * target, BytecodeEnum bytecode_name,
			ValueType source_type) override;
	};
}
