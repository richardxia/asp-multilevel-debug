from asp.codegen.cpp_ast import *
from asp.codegen.ast_tools import *

class TinyCuOptimizer(NodeTransformer):
    def optimize(self, cpp):
        return self.visit(cpp)


    def visit_For(self, node):
        # replace it with a OpenMP parallel loop
        
        if node.loopvar == "sum_i":
            # it is a sum loop, so need reduction clause in pragma
            pragma = "omp parallel for reduction(+:accum)"
        else:
            pragma = "omp parallel for"

        newnode = UnbracedBlock(contents=[cpp_ast.Pragma(pragma), 
                                          For(node.loopvar, node.initial, node.end, node.increment, node.body)])
        return newnode
        