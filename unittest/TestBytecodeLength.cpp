#include "stdafx.h"
#include "CppUnitTest.h"
#include "../coldlang/stdafx.h"
#include "../coldlang/BytecodeVectorReader.h"
#include "../coldlang/BytecodeLength.h"
#include "../coldlang/BytecodeClass.h"

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
				BytecodeClass::get_bytecode_length(EnumCallNative),
				(size_t)9);
		}
	};
}