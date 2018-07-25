import sys, os
import subprocess

shell_addr = sys.argv[1]

cmd = shell_addr

testcases = [
    (
        """
        a = '习近平'
        b = native_puts(a)
        """,
        ""
    ),
    (
        """
        b = native_puts('习大大')
        """,
        ""
    ),
]

def beautify(string, indent):
    line_list = string.split('\n')
    ret = []
    for line in line_list:
        ret.append(' '*indent + line.strip())

    return '\n'.join(ret)


for testcase in testcases:
    child = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.PIPE)
    input_str = testcase[0].strip() + '\n:q'
    (outs, errs) = child.communicate(input_str.encode('cp936'))
    print("for test case\n-------\n%s\n-------\n" % beautify(testcase[0], 0))
    print("-------\n")
    print(outs.decode('cp936'))
    print("-------")