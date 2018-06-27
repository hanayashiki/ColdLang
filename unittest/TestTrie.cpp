#include "stdafx.h"
#include "CppUnitTest.h"

#include "../coldlang/stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{
	TEST_CLASS(TestTrie)
	{
	public:

		TEST_METHOD(AddSequence)
		{
			Trie<TrieNode<wchar_t, 26>, wchar_t, 10> trie(L'a');
			trie.add_sequence(L"abcd", 4, 0);
			trie.add_sequence(L"bcde", 4, 1);
			trie.add_sequence(L"bef", 3, 2);

			Assert::AreEqual(trie.get_sequence_tag(L"abcd", 4), 0);
			Assert::AreEqual(trie.get_sequence_tag(L"bcde", 4), 1);
			Assert::AreEqual(trie.get_sequence_tag(L"bef", 3), 2);
			Assert::AreEqual(trie.get_sequence_tag(L"be", 2), -1);
			Assert::AreEqual(trie.get_sequence_tag(L"c", 1), -1);
			Assert::AreEqual(trie.get_sequence_tag(L"befgg", 5), -1);
		}



	};
}