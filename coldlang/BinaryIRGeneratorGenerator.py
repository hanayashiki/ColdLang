Head = """\
#include "stdafx.h"
#include "IRGenerator.h"
#include "BytecodeClass.h"

namespace IR {
	using namespace OperandType;
	using namespace BytecodeClass;
"""

Tail = """\
}
"""

BinaryIRGeneratorDeclarationTemplate = """\
        Symbol * %(node_name)s_reader(TreeNode * tn, bool head, Symbol * left_symbol, string && left_builder_name);
        void %(node_name)s_tail_reader(TreeNode * tn, Symbol * target_symbol);
"""

BinaryIRGeneratorTemplate = """\
    Symbol * IRGenerator::%(node_name)s_reader(TreeNode * tn, bool head, Symbol * left_symbol, string && left_builder_name)
    {
        Symbol * ret = nullptr;
        Symbol * new_left_symbol = nullptr;
        if (head)
        {
            Symbol * %(child_node_name)s = %(child_node_name)s_reader(tn->get_non_terminal(0), true, nullptr, "");
            if (tn->get_non_terminal(1)->get_builder_name() == "%(node_name)s_tail_empty")
            {
                ret = %(child_node_name)s;
                return ret;
            }
            else
            {
                if (%(child_node_name)s == nullptr)
                {
                    // will be overwrite, so we should store `Acc` somewhere
                    Variable * temp = temp_table_->lend();
                    new_left_symbol = temp;
                    EMIT(StoreAcc, bytecode_writer_, temp);
                }
                else
                {
                    new_left_symbol = %(child_node_name)s;
                }
            }
        }
        else
        {
            if (left_symbol == nullptr) {
                left_symbol = temp_table_->lend();
                EMIT(StoreAcc, bytecode_writer_, left_symbol);
            }
            Symbol * %(child_node_name)s = %(child_node_name)s_reader(tn->get_non_terminal(0), true, nullptr, "");
            if (%(child_node_name)s == nullptr)
            {
                %(child_node_name)s = temp_table_->lend();
                EMIT(StoreAcc, bytecode_writer_, %(child_node_name)s);
                temp_table_->revert(static_cast<Variable*>(%(child_node_name)s));
            }
            EMIT(LoadSymbolToAcc, bytecode_writer_, left_symbol);
            if (left_symbol->is_temp())
            {
                temp_table_->revert(static_cast<Variable*>(left_symbol));
            }
            %(binary_op_emitters)s
        }
        %(node_name)s_tail_reader(tn->get_non_terminal(1), new_left_symbol);
        return ret;
    }

    void IRGenerator::%(node_name)s_tail_reader(TreeNode * tn, Symbol * target_symbol)
    {
        if (tn->get_builder_name() != "%(node_name)s_tail_empty") {
            %(node_name)s_reader(tn->get_non_terminal(1), false, target_symbol, tn->get_builder_name());
        }
    }
"""

BinaryOpEmitterTemplate = """\
            if (left_builder_name == "%(left_builder_name)s")
                EMIT(%(op_name)s, bytecode_writer_, %(child_node_name)s);
"""

BinaryIRDefines = [
    {
        'node_name' : 'expr_4',
        'child_node_name' : 'expr_5',
        'builder_to_op' : [
            ('greater_equal_expr_4', 'GreaterEqual'),
            ('greater_expr_4', 'GreaterThan'),
            ('less_equal_expr_4', 'LessEqual'),
            ('less_expr_4', 'LessThan'),
        ]
    },
    {
        'node_name' : 'expr_3',
        'child_node_name' : 'expr_4',
        'builder_to_op' : [
            ('not_equal_expr_3', 'NotEqual'),
            ('equal_expr_3', 'Equal'),
        ]
    },
    {
        'node_name' : 'expr_2',
        'child_node_name' : 'expr_3',
        'builder_to_op' : [
            ('expr_2_tail_and', 'And')
        ]
    },
    {
        'node_name' : 'expr_1',
        'child_node_name' : 'expr_2',
        'builder_to_op' : [
            ('expr_1_tail_or', 'Or')
        ]
    }
]

def GenerateBinaryOpEmitterTemplate(left_builder_name : str, op_name : str, child_node_name : str):
    return BinaryOpEmitterTemplate % { 'left_builder_name': left_builder_name, 'op_name': op_name, 'child_node_name': child_node_name };

def GenerateBinaryOpEmitterList(builder_to_op : list, child_node_name : str):
    template_list = []
    for builder, op in builder_to_op:
        template_list.append(GenerateBinaryOpEmitterTemplate(builder, op, child_node_name))
    return ''.join(template_list)

def GenerateBinaryIRGeneratorTemplate(node_name : str, child_node_name : str, builder_to_op : list):
    return  BinaryIRGeneratorTemplate % {'node_name': node_name, 'child_node_name': child_node_name, 
        'binary_op_emitters': GenerateBinaryOpEmitterList(builder_to_op, child_node_name) }

def GenerateBinaryIRGeneratorTemplateList(binary_defines : dict):
    template_list = []
    for binary_define in binary_defines:
        template_list.append(
            GenerateBinaryIRGeneratorTemplate(binary_define['node_name'], binary_define['child_node_name'], binary_define['builder_to_op'])
        )
    return ''.join(template_list)

def GenerateBinaryIRGeneratorDeclaration(binary_defines : dict):
    declaration_list = []
    for binary_define in binary_defines:
        declaration_list.append(
            BinaryIRGeneratorDeclarationTemplate % { 'node_name': binary_define['node_name'] }
        )
    return ''.join(declaration_list)


print(GenerateBinaryIRGeneratorDeclaration(BinaryIRDefines))
print('-----------------------------------------------------')
print(Head)
print(GenerateBinaryIRGeneratorTemplateList(BinaryIRDefines))
print(Tail)

