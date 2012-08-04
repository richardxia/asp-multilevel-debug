import ast
import asp.codegen.cpp_ast as cpp_ast
import types
import collections

class NodeFinder(ast.NodeVisitor):
    '''Generic class for finding a specific node in an AST subtree.'''
    def __init__(self, predicate):
        self.predicate = predicate

    def generic_visit(self, node):
        if self.predicate(node):
            return node
        for field, value in ast.iter_fields(node):
            if isinstance(value, list):
                for item in value:
                    if isinstance(item, ast.AST):
                        tmp = self.visit(item)
                        if tmp:
                            return tmp
            elif isinstance(value, ast.AST):
                tmp = self.visit(value)
                if tmp:
                    return tmp
        return False

class AspTrace(object):
    def __init__(self, trace_file="/tmp/trace.txt"):
        self.trace_file = trace_file
        self.var_name = '_asp_log_file'

    def emit_open(self):
        block = cpp_ast.UnbracedBlock()
        block.append(cpp_ast.Value("std::ofstream", self.var_name))
        block.append(cpp_ast.FunctionCall("%s.open" % self.var_name, [cpp_ast.String(self.trace_file)]))
        return block

    def emit_close(self):
        return cpp_ast.FunctionCall("%s.close" % self.var_name, [])

    def emit_write(self, *args):
        return PrintLog.write_log(*args)

class SpecializationError(Exception):
    pass

class PrintLog(cpp_ast.Generable):
    def __init__(self, outfile, texts, newline):
        self.outfile = outfile
        self.texts = texts
        self.newline = newline
        self._fields = ['texts']

    def generate(self, with_semicolon=True):
        ss = self.outfile
        for item in self.texts:
            for item_line in item.generate():
                ss += ' << ' + item_line
        if self.newline:
            ss += ' << std::endl'
        ss += ';'
        yield ss

    @classmethod
    def write_log(cls, *args):
        outputs = [cpp_ast.String("_asp_log_write(")]
        args = [cpp_ast.String(x) if isinstance(x, str) else x for x in args]
        for x in args[:]:
            if isinstance(x, list):
                i = args.index(x)
                args[i:i+1] = x
        if len(args) > 0:
            outputs.append(args[0])
        if len(args) > 1:
            for arg in args[1:]:
                outputs.append(cpp_ast.String(', '))
                outputs.append(arg)
        outputs.append(cpp_ast.String(')'))
        return cls("_asp_log_file", outputs, True)

def instrument_deepcopy(mod):
    def new_deepcopy(self, memo):
        new_obj = self.__old_deepcopy__(memo)
        if hasattr(self, 'should_trace'):
            new_obj.should_trace = self.should_trace
        return new_obj

    for key, cls in mod.__dict__.iteritems():
        if type(cls) == type(object) and \
                cls not in types.__dict__.values() and \
                cls not in collections.__dict__.values() and \
                hasattr(cls, '__deepcopy__'):
            cls.__old_deepcopy__ = cls.__deepcopy__
            cls.__deepcopy__ = new_deepcopy
