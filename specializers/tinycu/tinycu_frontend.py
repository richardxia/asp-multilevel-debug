from asp.codegen.ast_tools import *
from tinycu_sm import *
import asp.codegen.python_ast as ast

class TinyCuFrontend(NodeTransformer):
    def parse(self, tree, rettype="double", locals={}):
        self.rettype = rettype
        self.locals = locals
        
        print "locals is ", locals

        # here, tree is a Module node, but we know the first item in it is a FunctionDef node
        return self.visit(tree.body[0])

    def visit_FunctionDef(self, node):
        return CuFunction(node.name, self.rettype, self.visit(node.args), self.visit(node.body[0]))

    def visit_arguments(self, node):
        return [self.visit(x) for x in node.args]

    def visit_Return(self, node):
        # we require a return value
        return FunctionReturn(self.visit(node.value))

    def visit_Call(self, node):
        # detect what the function is, and then return the correct SM node 
        if isinstance(node.func, python_ast.Name) and node.func.id == "map":
            return CuMap(self.visit(node.args[0]), [self.visit(x) for x in node.args[1:]])

        if isinstance(node.func, python_ast.Name) and node.func.id == "sum":
            return CuSum(self.visit(node.args[0]))

        if isinstance(node.func, python_ast.Name) and node.func.id == "reduce":
            return CuReduce(self.visit(node.args[0]), [self.visit(x) for x in node.args[1:]])

    def visit_Name(self, node):
        return Identifier(node.id)
    
    def visit_Num(self, node):
        return Constant(node.n)

    def visit_Attribute(self, node):
        val = self.visit(node.value)
        assert isinstance(val, Identifier) and val.name == "self"

        print "Checking if ", node.attr, "is in ", self.locals
        # if it is a constant value, then insert the constant
        if node.attr in self.locals.keys():
            return Constant(self.locals[node.attr])

        return Attribute(val, Identifier(node.attr))

    def visit_List(self, node):
        # turn into a CuList
        return CuList([self.visit(x) for x in node.elts])

    def visit_BinOp(self, node):
        return BinaryOp(self.visit(node.left),
                        node.op,
                        self.visit(node.right))
