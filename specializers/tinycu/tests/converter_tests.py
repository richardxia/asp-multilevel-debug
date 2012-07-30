import unittest2 as unittest
import ast, inspect
from tinycu_frontend import *
from tinycu_converter import *
from tinycu_sm import *

class BackendTests(unittest.TestCase):
    
    def test_for_map_node2(self):
        def call(self):
            return map(self.func, [1,2,3,4])

        sm = TinyCuFrontend().parse(ast.parse(inspect.getsource(call).lstrip()), "std::vector<double>*")
        
        cpp_ast = TinyCuConverter().convert(sm)
        print cpp_ast[0]
        print cpp_ast[1]
        
        self.assertEqual(cpp_ast[1], ["func"])

        wanted = """std::vector<double>* call()
{
  double tmp0_[] = {1,2,3,4};
  std::vector<double> *tmp0 = new std::vector<double>(tmp0_, tmp0_+sizeof(tmp0_)/sizeof(tmp0_[0]));
  return [&]()->std::vector<double>* 
{
  std::vector<double>* tmp0_inner = new std::vector<double>(*tmp0);
  for (int i = 0; (i <= tmp0->size()); i = (i + 1))
  {
    (*tmp0_inner)[i] = func((*tmp0)[i]);
  }
  return tmp0_inner;
}();
}
"""

        self.assertEqual(str(cpp_ast[0]), wanted)


    def test_specializing_simple_function(self):
        def func(self, a):
            return 2*a
        sm = TinyCuFrontend().parse(ast.parse(inspect.getsource(func).lstrip()))
        cpp_ast = TinyCuConverter().convert(sm)
        print cpp_ast[0]
        wanted="""double func(double a)
{
  return (2 * a);
}"""
        self.assertEqual(str(cpp_ast[0]), wanted)
        self.assertEqual(cpp_ast[1], [])


    def test_for_sum_node(self):
        def call(self):
            return sum([1,2,3,4])

        sm = TinyCuFrontend().parse(ast.parse(inspect.getsource(call).lstrip()), "std::vector<double>*")
        
        cpp_ast = TinyCuConverter().convert(sm)
        print cpp_ast[0]
        print cpp_ast[1]




class CppLambdaTests(unittest.TestCase):
    def test_basic(self):
        import asp.codegen.cpp_ast as cpp_ast
        node = cpp_ast.FunctionBody(CppLambda("", cpp_ast.Value("int",""),
                                              [cpp_ast.Value("int", "x")]),
                                    cpp_ast.Block(contents=[]))

        print node
        self.assertEqual(str(node),"[](int x)->int \n{\n}")
                                              
                                              

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

