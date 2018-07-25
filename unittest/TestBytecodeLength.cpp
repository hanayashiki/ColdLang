#include "stdafx.h"
#include "CppUnitTest.h"
#include "../coldlang/stdafx.h"
#include "../coldlang/BytecodeVectorReader.h"
#include "../coldlang/BytecodeEnum.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{
	using namespace IR;
	using namespace IR::BytecodeClass;
	TEST_CLASS(TestBytecodeLength)
	{
	public:
		TEST_METHOD(BytecodeLength)
		{
			Assert::AreEqual(
				BytecodeClass::get_bytecode_length(EnumCallNativeFunc),
				(size_t)9);
		}
	};
}