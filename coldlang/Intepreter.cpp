#include "stdafx.h"
#include "Intepreter.h"
#include "ColdLangBackend.h"
#include "BasicBlockHolder.h"
#include "RuntimeFunction.h"

namespace Cld {
	_Interpreter::_Interpreter(const wchar_t * code)
		: code(code)
	{
	}

	CldRuntime::RuntimeValue * _Interpreter::Main()
	{
		Tree* tree;
		ColdLangFrontEnv* env;
		ColdLangBackend* backend;
		IR::BasicBlockHolder* blocks;
		IR::Module* module;

		env = new ColdLangFrontEnv(&code);
		CLD_DEBUG << "parse started" << std::endl;
		tree = env->syntax->parse("statement_block");
		//std::wcout << tree->to_xml(100);

		backend = new ColdLangBackend();
		module = new IR::Module("test", nullptr, backend->symbol_table);
		backend->ir_gen->statement_block_reader(tree->get_root());

		blocks = new IR::BasicBlockHolder(backend->bytecode_reader);
		CLD_DEBUG << "make_blocks" << std::endl;
		blocks->make_blocks();
		// wcout << blocks->to_string();

		CldRuntime::RuntimeFunction rf(&blocks->get_blocks(), module->get_main());

		CLD_DEBUG << "rf run" << std::endl;
		rf.Run({});

		delete tree;
		delete env;
		delete backend;
		delete blocks;
		delete module;

		return rf.GetLastRetVal<CldRuntime::RuntimeValue>();
	}
}
