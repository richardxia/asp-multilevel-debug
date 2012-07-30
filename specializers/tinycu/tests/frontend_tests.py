import unittest2 as unittest
import ast, inspect
from tinycu_frontend import *
from tinycu_sm import *

class FrontendTests(unittest.TestCase):
    
    def test_for_map_node(self):
        def call(self):
            return map(self.func, [1,2,3,4])

        sm = TinyCuFrontend().parse(ast.parse(inspect.getsource(call).lstrip()))
        
        NodeDumper().visit(sm)
        assert_contains_node_type(sm, CuMap)

    def test_simple_function(self):
        def func(self, a):
            return 2*a
        
        sm = TinyCuFrontend().parse(ast.parse(inspect.getsource(func).lstrip()))
        
    def test_for_sum_node(self):
        def call(self):
            return sum([1,2,3])

        sm = TinyCuFrontend().parse(ast.parse(inspect.getsource(call).lstrip()))
        assert_contains_node_type(sm, CuSum)



def assert_contains_node_type(root_node, node_type):
    assert ContainsNodeTypeVisitor().contains(root_node, node_type), "Expected tree to contain node of type %s" % node_type

class ContainsNodeTypeVisitor(ast.NodeVisitor):
    def contains(self, root_node, node_type):
        self.node_type = node_type
        self.result = False
        self.visit(root_node)
        return self.result

    def visit(self, node):
        if isinstance(node, self.node_type):
            self.result = True
        self.generic_visit(node)


class NodeDumper(ast.NodeVisitor):
    def visit(self, node):
        print node
        self.generic_visit(node)
        


if __name__ == '__main__':
    unittest.main()

