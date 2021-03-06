// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS


#include "targetver.h"

#include "../asmjit/asmjit.h"

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <exception>
#include <regex>
#include <iostream>
#include <vector>
#include <assert.h>
#include <typeinfo>

using namespace std;

#include "Trie.h"
#include "ResizableBuffer.h"
#include "KeywordTrie.h"
#include "Token.h"
#include "String.h"
#include "Word.h"
#include "Integer.h"
#include "Float.h"
#include "Delimiter.h"
#include "DelimiterParser.h"
#include "NumberParser.h"
#include "StringParser.h"
#include "TokenException.h"
#include "Lexer.h"
#include "TreeBuilderNode.h"
#include "TreeMeta.h"
#include "Tutils.h"
#include "Tree.h"
#include "BadSyntaxException.h"
#include "Syntax.h"
#include "ModuleConfig.h"
#include "ColdLangFrontEnv.h"

#include "CLObject.h"

#include "Symbol.h"
#include "Label.h"
#include "Variable.h"
#include "TempTable.h"
#include "SymbolTable.h"
#include "LiteralTable.h"
#include "SymbolException.h"
#include "Function.h"
#include "FunctionTable.h"
#include "IRGenerator.h"
#include "BytecodeClass.h"

#include "Module.h"

// TODO: 在此处引用程序需要的其他头文件
