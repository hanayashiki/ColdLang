#pragma once
#include "stdafx.h"

template<typename T, int N>
struct TrieNode
{
	T value_;
	int tag_;
	TrieNode* next_[N];
	bool is_leaf_;
	TrieNode() {
		is_leaf_ = true;
		value_ = 0;
		tag_ = -1;
		memset(next_, 0, N * sizeof(TrieNode*));
	}
	TrieNode(T value) : value_(value) {
		is_leaf_ = true;
		tag_ = -1;
		memset(next_, 0, N * sizeof(TrieNode*));
	};
};

// T: the type of the TrieNode
// S: the type of the sequence, such as char/wchar_t
// M: the total count of units of the sequences
template<typename T, typename S, int M>
class Trie {
private:
	T trie_node_[M + 1];
	T* root_;
	int trie_node_pointer_;
	S base_;
public:
	Trie(S base): base_(base) {
		trie_node_[0] = T(0);
		root_ = &(trie_node_[0]);
		trie_node_pointer_ = 1;
	}

	Trie() : base_(0) {
		trie_node_[0] = T(0);
		root_ = &(trie_node_[0]);
		trie_node_pointer_ = 1;
	}

	void add_sequence(const S sequence[], int length, int leaf_tag = 0) {
		if (length <= 0) {
			return;
		}
		T* node = root_;
		assert(root_ == &(trie_node_[0]));
		for (int i = 0; i < length; i++) {
			if (node->next_[sequence[i] - base_] == NULL) { // node does not exist
				//wcout << sequence[i] << endl;
				// wcout << L"unset leaf: " << (node - root_) << endl;
				node->next_[sequence[i] - base_] = &trie_node_[trie_node_pointer_];
				trie_node_[trie_node_pointer_++].value_ = sequence[i];
			}
			node = node->next_[sequence[i] - base_]; // visit existing child node;
		}
		node->tag_ = leaf_tag;
		assert(trie_node_pointer_ <= M + 1);
	}

	int get_sequence_tag(const S sequence[], int length) {
		T* node = root_;
		if (length <= 0) {
			return -1;
		}
		for (int i = 0; i < length; i++) {
			if (sequence[i] - base_ < 0 || sequence[i] - base_ >= sizeof(trie_node_[0].next_) / sizeof(trie_node_[0].next_[0])) {
				return -1;
			}
			if (node->next_[sequence[i] - base_] == NULL) { // node does not exist
				return -1;
			}
			node = node->next_[sequence[i] - base_];
		}
		return node->tag_;
	}
};