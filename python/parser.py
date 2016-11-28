import sys
import re
import logging
from collections import defaultdict
from json import loads

class Program(object):
    def parse(self, program_code):
        def strip_program(program):
            program = program.lower()
            program = re.sub(r"\s\s+"," ", program).replace('\n', ' ')
            program = program.split(';')
            program = list(map(lambda x: x.strip(), program))
            return program

        def tokenize(program):
            tokens = []
            for line in program:
                if line.startswith('while'):
                    do_index = line.find(' do')
                    tokens.append(line[:do_index+ len(' do')])
                    tokens.append(line[do_index + len(' do'):])
                elif line.endswith('end'):
                    end_index = line.find(' end')
                    tokens.append(line[:end_index])
                    tokens.append(line[end_index+1:])
                else:
                    tokens.append(line)
            return [token.replace(' ', '') for token in tokens]

        program = strip_program(program_code)
        tokens = tokenize(program)
        return tokens

    @staticmethod
    def get_variable_index(var_string):
        var_index = var_string[1:]
        var_index = int(var_index)
        return var_index

    def __init__(self, program_code):
        self.tokens = self.parse(program_code)
        self.curr_pos = 0
        self.result = None

    def move_forward(self):
        self.curr_pos += 1

    def move_backward(self):
        self.curr_pos -= 1

    def goto_while_end(self):
        open_loops = 1
        while open_loops != 0:
            self.move_forward()
            if self.is_while_begin():
                open_loops += 1
            elif self.is_while_end():
                open_loops -= 1
            else:
                pass

    def goto_while_begin(self):
        open_loops = 1
        while open_loops != 0:
            self.move_backward()
            if self.is_while_end():
                open_loops += 1
            elif self.is_while_begin():
                open_loops -= 1
            else:
                pass

    def is_while_begin(self):
        line = self.tokens[self.curr_pos]
        return line.startswith('while')

    def extract_condition_variable(self):
        line = self.tokens[self.curr_pos]
        condition_begin = len('while')
        condition_end   = len(line) - len('/=0do')
        return line[condition_begin:condition_end]

    def check_while_condition(self):
        index = self.get_variable_index(self.extract_condition_variable())
        return self.variables[index] != 0

    def is_while_end(self):
        line = self.tokens[self.curr_pos]
        return line == 'end'

    def split_assignment(self):
        line = self.tokens[self.curr_pos]
        left_variable_end = line.find(':=')
        # *c with * in {+,-} and c in {0,1}
        constant_begin    = len(line) - len('*c')
        left_variable = line[:left_variable_end]
        constant      = int(line[constant_begin:])
        assigned_variable = line[left_variable_end + len(':='):constant_begin]
        # account for not writing +-1 but -1
        assigned_variable = assigned_variable.replace('+', '')
        # print("(left_variable: {0}, assigned_variable: {1}, constant: {2})"
        #        .format(left_variable, assigned_variable, constant))
        return (self.get_variable_index(left_variable),
                self.get_variable_index(assigned_variable),
                constant)

    def execute(self, input_variables):
        self.variables = defaultdict(int, input_variables)
        self.curr_pos = 0
        self.result = None
        while self.curr_pos < len(self.tokens):
            if self.is_while_begin():
                if not self.check_while_condition():
                    self.goto_while_end()
                self.move_forward()
            elif self.is_while_end():
                self.goto_while_begin()
            else:
                (left_index, value_index, constant) = self.split_assignment()
                self.variables[left_index] = max(0,
                        self.variables[value_index] + constant)
                self.move_forward()
        self.result = self.variables[0]

if (len(sys.argv) != 2):
    print("usage: {0} <filename>".format(sys.argv[0]))
else:
    file_name = sys.argv[1]
    with open(file_name) as f:
        file_content = f.read()
    content = loads(file_content)
    if not {'program', 'tests'}.issubset(content.keys()):
        print("file must contain 'program' and 'tests' values")
    else:
        prog_file = content['program']
        with open(prog_file) as p:
            prog_code = p.read()
        prog = Program(prog_code)
        test_cases = content['tests']
        for case in test_cases:
            if not 'input' in case.keys():
                print("testcase {0} must contain 'input' values".format(
                    test_cases.index(case)))
            else:
                show_if_correct = case.get('show_if_correct', False)
                result          = case.get('result', None)
                inputs_list = case['input']
                inputs = {inputs_list.index(i) + 1 : i for i in inputs_list}
                prog.execute(inputs)
                if not result or result != prog.result or show_if_correct:
                    print("inputs {0} led to {1}, expected {2}".format(
                        inputs, prog.result, result))
