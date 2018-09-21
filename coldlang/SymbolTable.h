#pragma once
#include "stdafx.h"
#include "NativeSymbol.h"

namespace IR
{
	template <class _Tp>
	struct SymbolEqual
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
	public:
		explicit SymbolTable(SymbolTable * parent = nullptr);
		void add(OperandType::Variable* symbol);
		void add(OperandType::NativeSymbol * native);
		void add_symbol(const wchar_t * name, OperandType::Symbol * symbol);
		bool define(OperandType::Variable* symbol);
		void mock(initializer_list<const wchar_t *> init_list);
		
		size_t add_sub_symbol_table();
		SymbolTable* get_sub_symbol_table(size_t id);
		SymbolTable* get_parent_symbol_table();

		OperandType::Symbol * get_by_name(const wchar_t*);
		OperandType::Symbol * get_by_id(size_t id);
		wstring dump_to_string(size_t offset = 0);

		size_t get_variable_count() const { return var_id; }

		~SymbolTable();
	private:
		static size_t symbol_id;
		size_t var_id;

		SymbolTable * parent_;
		uint32_t nest_level_;
		vector<SymbolTable*> sub_symbol_tables_;
		unordered_map<const wchar_t*, OperandType::Symbol*, SymbolHashFunc, SymbolEqual<const wchar_t*>> name_to_symbol_;
		unordered_map<size_t, OperandType::Symbol*> id_to_symbol_;

		inline void init_native_symbols();
	};
}