"""#Takes a Python AST and converts it into a Python AST with logging.
Takes a StencilModel and transforms it back into a Python AST but with rigged iterators
Maybe take a PythonAST only only transform the parts necessary?
This is kind of hard...
"""

import asp.codegen.python_ast as ast

# Converts from stencil coords (x,y,z) into cpp coords (one-dimesional)
def py2cpp(coord, shape):
    grid_coord = 0
    for i in range(len(shape)):
        grid_coord = shape[i] * grid_coord +  coord[i]
    return grid_coord

def cpp2py(coord, shape):
    grid_coord = []
    for i in reversed(range(len(shape))):
        rem = coord % shape[i]
        grid_coord.append(rem)
        coord = (coord - rem)/shape[i]
    return tuple(reversed(grid_coord))

class LogReader(object):
    def __init__(self, logfile):
        self.log = {}
        self.parse(logfile)

        self.current_value = None

    def parse(self, logfile):
        import re
        f = open(logfile)
        for line in f:
            tmp = re.search(r"\((.*)\)", line)
            if tmp is not None:
                self.add_to_log([x.strip() for x in tmp.group(1).split(",")])

    def add_to_log(self, args):
        target, target_idx, input_idx, value = args
        target_idx = int(target_idx)
        input_idx = int(input_idx)
        value = float(value)
        if target_idx not in self.log:
            self.log[target_idx] = []
        self.log[target_idx].append((input_idx, value))

    def getIterator(self):
        def tmp(grid, *args):
            grid_coord = py2cpp(args[0], grid.shape)

            for l in self.log[grid_coord]:
                coord, value = l
                self.current_value = value
                yield cpp2py(coord, grid.shape)
        return tmp

def _log_read():
    return LogReader("/tmp/trace.txt")

# Get rid of this???

class StencilLoggingTransform(ast.NodeTransformer):
    '''Transforms a Python AST into an AST with Nondeterministic For loops annotated.'''
    # Is this all I'm supposed to do?
    def parse(self, node):
        self.iterators = []
        tmp = self.visit(node)
        ast.fix_missing_locations(tmp)
        return tmp

    def visit_FunctionDef(self, node):
        self.generic_visit(node)
        imp = ast.ImportFrom("stencil_logging_transform", [ast.alias("_log_read", None)], 0)
        log_open = ast.Assign(
            [ast.Name("_logs", ast.Store())],
            ast.Call(
                ast.Name("_log_read", ast.Load()),
                [],[],None,None))
        node.body.insert(0, log_open)
        node.body.insert(0, imp)
        return node

    def visit_For(self, node):
        args = [ast.Str(node.iter.func.value.id)]
        for iter in self.iterators:
            args.append(ast.Name(iter.target.id, ast.Load()))
        node.iter = ast.Call(
            ast.Attribute(
                ast.Name("_logs", ast.Load()),
                "getIterator",
                ast.Load()),
            args,
            [],None,None)
        self.iterators.append(node)
        self.generic_visit(node)
        self.iterators.pop()
        return node
