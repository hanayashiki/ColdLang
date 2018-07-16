#pragma once
#include "stdafx.h"

namespace IR
{
	template <class _Tp>
	struct SymbolEqual : public binary_function<_Tp, _Tp, bool>
	{
		bool operator()(const _Tp& __x, const _Tp& __y) const
		{
			return wcscmp(__x, __y) == 0;
		}
	};

	class SymbolHashFunc
	{
	public:
		size_t operator() (const wchar_t * str) const {
			const static size_t base = 19260817;
			size_t ans = 0;
			while (*str != L'\0')
			{
				ans = ans * base + *str;
				str++;
			}
			return ans;
		}

	};
	class SymbolTable
	{
	private:
		static size_t symbol_id;
		SymbolTable * parent_;
		vector<SymbolTable*> sub_symbol_tables_;
		unordered_map<const wchar_t*, OperandType::Symbol*, SymbolHashFunc, SymbolEqual<const wchar_t*>> name_to_symbol_;
		unordered_map<size_t, OperandType::Symbol*> id_to_symbol_;
	public:
		SymbolTable(SymbolTable * parent = nullptr);
		void add(OperandType::Variable* symbol);
		void mock(initializer_list<const wchar_t *> init_list);
		OperandType::Symbol * get_by_name(const wchar_t*);
		OperandType::Symbol * get_by_id(size_t id);
		~SymbolTable();
	};
}