﻿#pragma once
#include "stdafx.h"
#include "Word.h"
#include "Trie.h"

template<typename T, int N>
struct TrieNode;

template<typename T, typename S, int M>
class Trie;

class KeywordTrie {
private:
	const static int TRIE_SIZE = 54;
	Trie<TrieNode<wchar_t, 26>, wchar_t, TRIE_SIZE> trie = 
		Trie<TrieNode<wchar_t, 26>, wchar_t, TRIE_SIZE>(L'a');
public:
	KeywordTrie() {
		trie.add_sequence(L"fn", 2, Word::keyword_fn);
		trie.add_sequence(L"class", 5, Word::keyword_class);
		trie.add_sequence(L"struct", 6, Word::keyword_struct);
		trie.add_sequence(L"use", 3, Word::keyword_use);
		trie.add_sequence(L"if", 2, Word::keyword_if);
		trie.add_sequence(L"while", 5, Word::keyword_while);
		trie.add_sequence(L"for", 3, Word::keyword_for);
		trie.add_sequence(L"return", 6, Word::keyword_return);
		trie.add_sequence(L"task", 4, Word::keyword_task);
	}

	int get_tag(wchar_t* wstr, int length) {
		return trie.get_sequence_tag(wstr, length);
	}
};