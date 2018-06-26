#pragma once
#include "stdafx.h"
#include "Word.h"

class KeywordTrie {
private:
	static Trie<TrieNode<wchar_t, 26>, wchar_t, 48> trie;
public:
	KeywordTrie() {
		trie = Trie<TrieNode<wchar_t, 26>, wchar_t, 48>(L'a');
		trie.add_sequence(L"fn", 2, Word::keyword_fn);
		trie.add_sequence(L"class", 5, Word::keyword_class);
		trie.add_sequence(L"struct", 6, Word::keyword_struct);
		trie.add_sequence(L"use", 3, Word::keyword_use);
		trie.add_sequence(L"if", 2, Word::keyword_if);
		trie.add_sequence(L"while", 5, Word::keyword_while);
		trie.add_sequence(L"for", 3, Word::keyword_for);
		trie.add_sequence(L"return", 6, Word::keyword_return);
	}

	int get_tag(wchar_t* wstr, int length) {
		return trie.get_sequence_tag(wstr, length);
	}
};