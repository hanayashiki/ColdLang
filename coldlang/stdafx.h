// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <exception>
#include <regex>
#include <iostream>
#include <vector>
#include <assert.h>
#include <typeinfo>

#include "Trie.h"
#include "ResizableBuffer.h"
#include "KeywordTrie.h"
#include "Module.h"
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

#include "BytecodeOperandTypes.h"
#include "BytecodeDef.h"
#include "BytecodeVirtualClass.h"

#include "Symbol.h"
#include "Variable.h"
#include "TempTable.h"
#include "SymbolTable.h"

// TODO: 在此处引用程序需要的其他头文件
