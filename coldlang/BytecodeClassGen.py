
bytecode_classes = []

acc_read = 'AccumulatorUse::Read'
acc_write = 'AccumulatorUse::Write'
acc_none = 'AccumulatorUse::None'
acc_read_write = 'AccumulatorUse::ReadWrite'

variable = 'OperandType::Variable*'
literal = 'OperandType::Literal*'
label = 'OperandType::Label*'
native_func = 'OperandType::NativeFunction*'

head =\
"""\
#pragma once
#include "stdafx.h"
#include "BytecodeByteDump.h"
#include "Variable.h"
#include "BytecodeBase.h"
#include "BytecodeWriter.h"
#include "IRGenerator.h"
#include "Literal.h"
#include "Label.h"

/* this is auto-generated by BytecodeClassGen.py */

namespace IR
{
	namespace BytecodeClass
	{
"""

tail =\
"""\
    }
}
"""

enums =\
"""\
        enum %(enum_name)s {
%(enum_list)s        };
"""

enums_to_string =\
"""\
        static const char * %(name)s[] = {
%(name_list)s        };
"""

class_template =\
"""\
		class %(bytecode_name)s : public %(base_class)s
		{
		private:
			unsigned char byte_buf[128];
            size_t byte_len;
            %(bytecode_param_list)s;
		public:
            static const unsigned char code_id = Enum%(bytecode_name)s;
			%(bytecode_name)s(%(bytecode_param_list)s) 
                %(init_list)s
            {
                byte_len = params_to_byte(byte_buf, Enum%(bytecode_name)s%(bytecode_param_name_list)s);
            }

            size_t dump_byte(unsigned char out_buf[]) override
            {
                memcpy(out_buf, byte_buf, byte_len);
			    return byte_len;
            }

            wstring get_name() override
            {
                return L"%(bytecode_name)s";
            }

			wstring to_string() override
			{
				return get_name() + %(serialize_params)s;
			}%(additional_functions)s
		};
"""

head = head.replace('\t', ' '*4)
tail = tail.replace('\t', ' '*4)
class_template = class_template.replace('\t', ' '*4)

emit_template_declare =\
"""\
		template<typename VCode>
		inline void emit(BytecodeWriter * bw, OperandType::Symbol* param);
""".replace('\t', ' '*4)

emit_template_define =\
"""\
		template<>
		inline void emit<%(virtual_name)s>(BytecodeWriter * bw, OperandType::Symbol* param)
		{
%(type_select_list)s
		}\
""".replace('\t', ' '*4)

emit_type_selete_template =\
"""\
            if (auto t = dynamic_cast<%(op_type)s>(param))
			{
				EMIT(%(bytecode_name)s, bw, t);
			}
""".replace('\t', ' '*4)

virtual_bytecode_class_template =\
"""\
        class %s {};\
""".replace('\t', ' '*4)

acc_use =\
"""
namespace IR
{
	namespace AccumulatorUse
	{
		enum Type {
			None = 0,
			Read = 1 << 0,
			Write = 1 << 1,
			ReadWrite = Read | Write
		};
	}
}

""".replace('\t', ' '*4)

bytecode_list_header =\
"""\
#define BYTECODE_LIST(C)\\"""

bytecode_list_entry_template =\
"""\
        C(%(bytecode_name)s, %(acc_use)s%(arg_list)s)\\""".replace("\t", 4*" ")

get_unary_template0=\
"""
            template<T>
			static UnaryBytecodeInfo get_unary_info<%(class_name)s>(unsigned char buf[]);
"""

get_unary_template=\
"""
		static UnaryBytecodeInfo get_unary_info_%(class_name)s(unsigned char buf[])
		{
			return {
				%(op_type)s, retrieve_arg<%(arg_type)s>(&buf[1])
			};
		}
""".replace("\t", 4*" ")

get_unary_info_array=\
"""
        typedef UnaryBytecodeInfo (*GetUnaryInfo)(unsigned char[]);
        const GetUnaryInfo get_unary_info_list[] = {
%(get_unary_info_list)s
        };

        static GetUnaryInfo get_get_unary_info(unsigned char buf[]) {
            unsigned char code_id = buf[0];
            GetUnaryInfo getter = get_unary_info_list[code_id];
            assert(getter != nullptr);
            return getter;
        }

        static bool is_unary(const unsigned char buf[]) {
            unsigned char code_id = buf[0];
            return code_id >= %(first_unary)s::code_id && code_id <= %(last_unary)s::code_id;
        }
        \
""".replace("\t", 4*" ")

emit_template_list = []
virtual_bytecode_class = []
bytecode_list = []
operator_type_list = []
get_unary_info_functions = []
unary_classes = []

def get_bytecode_enums():
    enum_list = [' ' * 12 + 'Enum%s,\n' % c['class_name'] for c in bytecode_classes]    
    return enums % {
        'enum_list': ''.join(enum_list),
        'enum_name': 'BytecodeEnum'
    }

def get_op_enums():
    enum_list = [' ' * 12 + '%s,\n' % op for op in operator_type_list]    
    return enums % {
        'enum_list': ''.join(enum_list),
        'enum_name': 'OpType'
    }

def get_op_enums_name():
    name_list = ''.join([ 12 * ' ' + '"%s",\n' % (e) for e in operator_type_list ])
    return enums_to_string % { 'name': 'OpTypeName', 'name_list': name_list }
    
def generate_class_from_param(bytecode_class):
    class_name = bytecode_class['class_name']
    acc_use = bytecode_class['acc_use']
    op_types = bytecode_class['op_types']
    base_class = bytecode_class['base_class']
    additional_functions = bytecode_class['additional_functions']

    parameter_list = generate_parameter_list(op_types)
    name_list = generate_name_list(op_types)
    init_list = generate_init_list(name_list, name_list)

    bytecode_list.append(bytecode_list_entry_template % {
        'bytecode_name': class_name,
        'acc_use': acc_use,
        'arg_list': '' if len(op_types) == 0 else ', ' + ', '.join(op_types)
    })
    # print(name_list)
    return class_template % {
        'bytecode_name': class_name,
        'bytecode_param_list': ', '.join(parameter_list),
        'bytecode_param_name_list': '' if len(name_list) == 0 else ', ' + ', '.join(name_list),
        'serialize_params': string_sum(name_list),
        'init_list': '' if len(init_list) == 0 else ': ' + init_list,
        'base_class': base_class,
        'additional_functions': additional_functions
    }

def bytecode_def(class_name: str, acc_use: str, op_types: list = None, base_class: str = "BytecodeBase", additional_functions: str = ''):
    bytecode_classes.append({
        'class_name': class_name,
        'acc_use': acc_use,
        'op_types': [] if op_types == None else op_types,
        'base_class': base_class,
        'additional_functions': additional_functions
    })


def bytecode_def_unary(class_prefix: str, acc_use: str, operator_name: str):
    b = bytecode_def

    virtual_bytecode_class.append(virtual_bytecode_class_template % class_prefix)

    if operator_name not in operator_type_list:
        operator_type_list.append(operator_name);

    type_select_list = []

    for suffix, op_type in [
        ('Variable', variable), ('Literal', literal)
        ]:

        b(class_prefix + suffix, acc_use, op_types=[op_type], base_class="BytecodeUnary")

        get_unary_function = get_unary_template % { 'class_name': class_prefix + suffix,
             'op_type': operator_name, 'arg_type': op_type }
        get_unary_info_functions.append(get_unary_function)
        unary_classes.append(class_prefix + suffix)

        type_select_list.append(emit_type_selete_template %
        {
            'bytecode_name': class_prefix + suffix,
            'op_type': op_type
        })
    
    
    emit_template_list.append(
        emit_template_define % {
            'virtual_name': class_prefix,
            'type_select_list': ''.join(type_select_list),
        }
    )


def generate_parameter_list(op_types: list, prefix = "param"):
    parameter_list = []
    for id, op in enumerate(op_types):
        parameter_list.append("%s %s" % (op, prefix + str(id+1)))
    return parameter_list


def generate_name_list(op_types: list, prefix = "param"):
    parameter_list = []
    for id, op in enumerate(op_types):
        parameter_list.append("%s" % (prefix + str(id+1)))
    return parameter_list


def generate_init_list(names: list, params: list):
    pairs = []
    for i in range(len(names)):
        name = names[i]
        param = params[i]
        pairs.append('%s(%s)' % (name, param))
    return ', '.join(pairs)


def string_sum(param_name_list):
    if len(param_name_list) > 0:
        return 'L" " + ' + ' + L" " + '.join([x + '->to_string()' for x in param_name_list])
    else:
        return 'L""'

get_enum_name_base = """\
        static wstring get_enum_name(BytecodeEnum bytecode_enum) {
            switch (bytecode_enum) {
%(get_enum_name_list)s            default:
                assert(false);
            }
        }
""".replace('\t', '    ')

single_get_enum_name = """\
 			case Enum%(bytecode_name)s:
				return L"%(bytecode_name)s";
				break;   
"""

def get_enum_name(bytecode_classes: list):
    single_get_enum_names = []
    for bytecode_def in bytecode_classes:
        single_get_enum_names.append(
            single_get_enum_name % {
                'bytecode_name': bytecode_def['class_name']
            }
        )
    return get_enum_name_base % {
        'get_enum_name_list': ''.join(single_get_enum_names)
    }

decompile_base = """\
		static wstring decompile(const unsigned char buf[])
		{
			wstring str;
			switch (buf[0])
			{
%(decompiler_list)s            default:
                assert(false);
			}
            return str;
		}
""".replace('\t', '    ')

single_decompiler = """\
			case Enum%(bytecode_name)s:
				str = get_enum_name(Enum%(bytecode_name)s)%(retrieve_list)s;
				break;
""".replace('\t', '    ')

def decompile(bytecode_classes: list):
    single_decompilers = []
    for bytecode_def in bytecode_classes:
        single_decompilers.append(
            single_decompiler % {
                'bytecode_name': bytecode_def['class_name'],
                'retrieve_list': generate_retrieve_list(bytecode_def)
            }
        )
    return decompile_base % {
        'decompiler_list': ''.join(single_decompilers)
    }

def generate_retrieve_list(bytecode_def: dict):
    parameter_type_list = bytecode_def['op_types']
    if len(parameter_type_list) > 0:
        # currently we have one-argument only bytecode
        # TODO: make it for multiple args
        return " + L\" \" + retrieve_arg<%s>(buf + 1)->to_string()" % parameter_type_list[0]
    else:
        return ""

def define():
    b = bytecode_def
    bu = bytecode_def_unary
    bu('Or', acc_read_write, "OpLogicalOr")
    bu('And', acc_read_write, "OpLogicalAnd")
    bu('Equal', acc_read_write, "OpEqual")
    bu('NotEqual', acc_read_write, "OpNotEqual")
    bu('GreaterEqual', acc_read_write, "OpGreaterEqual")
    bu('LessEqual', acc_read_write, "OpLessEqual")
    bu('GreaterThan', acc_read_write, "OpGreaterThan")
    bu('LessThan', acc_read_write, "OpLessThan")
    bu('Add', acc_read_write, "OpAdd")
    bu('Sub', acc_read_write, "OpSub")
    bu('Mul', acc_read_write, "OpMul")
    bu('Div', acc_read_write, "OpDiv")
    bu('Mod', acc_read_write, "OpMod")
    bu('Neg', acc_write, "OpNeg")
    bu('LoadToAcc', acc_write, "OpMove")
    b('PushParamVariable', acc_none, [variable])
    b('PushParamLiteral', acc_none, [literal])
    b('StoreAcc', acc_read, [variable])
    b('CallFunc', acc_write, [variable])
    b('CallNative', acc_write, [native_func])
    b('RetAcc', acc_read)
    b('Jump', acc_none, [label])
    b('JumpOnTrue', acc_read, [label])
    b('JumpOnFalse', acc_read, [label])
    b('Inc', acc_write, [variable])
    b('Decre', acc_write, [variable])

    b('Label', acc_none, [label])

define()

print(head)
print(get_bytecode_enums())
print(get_op_enums())
print(get_op_enums_name())
for bytecode_class in bytecode_classes:
    print(generate_class_from_param(bytecode_class))
for virtual_class in virtual_bytecode_class:
    print(virtual_class)
print(emit_template_declare)
for emit_template in emit_template_list:
    print(emit_template)
print(bytecode_list_header)
for bytecode in bytecode_list:
    print(bytecode)
for get_unary_info_function in get_unary_info_functions:
    print(get_unary_info_function)
unary_func_pointers = []
for bytecode in bytecode_classes:
    if bytecode['class_name'] in unary_classes:
        unary_func_pointers.append("get_unary_info_%s" % bytecode['class_name'])
    else:
        unary_func_pointers.append("nullptr")

print(
    get_unary_info_array % {
        'get_unary_info_list': ',\n'.join([12*' ' + x for x in unary_func_pointers]),
        'first_unary': unary_classes[0],
        'last_unary': unary_classes[-1]
    }
)
print("")

print(get_enum_name(bytecode_classes))
print(decompile(bytecode_classes))
print(tail)

print(acc_use)