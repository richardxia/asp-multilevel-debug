from asp.codegen.ast_tools import *
from tinycu_sm import *
import asp.codegen.python_ast as python_ast
import asp.codegen.cpp_ast as cpp_ast

class CppLambda(cpp_ast.FunctionBody):
    def __init__(self, capture, rettype, args):
        self.rettype = rettype
        self.args = args
        self.capture = capture
        self._fields = ['capture', 'rettype', 'args']

    def generate(self, with_semicolon=False):
        yield "[" + str(self.capture) + "]" + "(" + ",".join([str(x)[0:-1] for x in self.args]) + ")" + "->" + str(self.rettype)[0:-1]

class TinyCuConverter(NodeTransformer):
    def convert(self, sm):
        # other_funcs is a list of other functions referenced by the current function.
        # the top-level class will ensure these are all translated
        self.other_funcs = []

        # keep track of a sequence to ensure unique temporary names
        self.seq = 0

        return self.visit(sm), self.other_funcs

    def get_next_seq(self):
        self.seq += 1
        return self.seq-1


    def visit_CuFunction(self, node):
        # new idea for this: should keep track of needed intermediates, and generate them as well
        self.intermediates = []
        if node.name == "__call__":
            inputtype = "std::vector<double>*"
        else:
            inputtype = "double"
        return cpp_ast.FunctionBody(cpp_ast.FunctionDeclaration(cpp_ast.Value(node.returntype, node.name),
                                                                [cpp_ast.Value(inputtype, x.name) for x in node.inputs[1:]]),
                                                                cpp_ast.Block(contents=self.intermediates+
                                                                                        [self.visit(node.body)]))


    def visit_FunctionReturn(self, node):
        return cpp_ast.ReturnStatement(self.visit(node.value))

    def visit_CuMap(self, node):
        # we're going to build a C++ lambda here and call it
        print "IN CUMAP"
        print node.func
        names = [self.visit(x) for x in node.data]
        name = names[0]
        inner_name = str(name)+"_inner"
        block = [
            cpp_ast.Assign(cpp_ast.Value("std::vector<double>*", inner_name), cpp_ast.Call("new std::vector<double>", ["*"+str(name)])),
            cpp_ast.For("i",
                            0,
                            cpp_ast.BinOp(cpp_ast.Call(str(name)+"->size",[]), "-", cpp_ast.CNumber(1)),
                            1,
                            cpp_ast.Block(contents=[
                                cpp_ast.Assign(cpp_ast.Subscript(cpp_ast.CName("(*"+inner_name+")"), "i"),
                                               cpp_ast.Call(self.visit(node.func), [
                                                   # we can have multiple arity here
                                                   cpp_ast.Subscript("(*"+str(n)+")", "i") for n in names]))
                                                   ])),
            cpp_ast.ReturnStatement(cpp_ast.CName(inner_name))
        ]

        return cpp_ast.Call(cpp_ast.FunctionBody(CppLambda("&", cpp_ast.Value("std::vector<double>*", ""), []),
                                    cpp_ast.Block(contents=block)),[])


    def visit_CuSum(self, node):
        # build a C++ lambda and call it
        # but we don't return a single item, we return a vector with a single item in it
        
        # can be a list, a name, or a map
        if isinstance(node.data, CuList) or isinstance(node.data, Identifier):
            name = self.visit(node.data)
            inner_name = str(name)+"_inner"
            block = [
                cpp_ast.Assign(cpp_ast.Value("std::vector<double>*", inner_name), "new std::vector<double>"),
                #            cpp_ast.Call(cpp_ast.CName(inner_name+"->push_back"), [cpp_ast.CNumber(0.0)]),
                cpp_ast.Assign(cpp_ast.Value("double", "accum"), cpp_ast.CNumber(0.0)),
                cpp_ast.For("sum_i",
                            0,
                            cpp_ast.BinOp(cpp_ast.Call(str(name)+"->size", []), "-", cpp_ast.CNumber(1)),
                            1,
                            cpp_ast.Block(contents=[
                                cpp_ast.Assign(#cpp_ast.Subscript("(*"+inner_name+")", cpp_ast.CNumber(0)),
                                    cpp_ast.CName("accum"),
                                    cpp_ast.BinOp(cpp_ast.CName("accum"), #cpp_ast.Subscript("(*"+inner_name+")", cpp_ast.CNumber(0)),
                                                  "+",
                                                         cpp_ast.Subscript("(*"+str(name)+")", cpp_ast.CName("sum_i"))))])),
                cpp_ast.Call(cpp_ast.CName(inner_name+"->push_back"), [cpp_ast.CName("accum")]),
                cpp_ast.ReturnStatement(cpp_ast.CName(inner_name))
                
               ]

            return cpp_ast.Call(cpp_ast.FunctionBody(CppLambda("&", cpp_ast.Value("std::vector<double>*", ""), []),
                                                     cpp_ast.Block(contents=block)),[])


        if isinstance(node.data, CuMap):
            name = cpp_ast.CName("tmp"+str(self.get_next_seq()))
            inner_name = str(name)+"_inner"
            block = [
                cpp_ast.Assign(cpp_ast.Value("std::vector<double>*", inner_name), "new std::vector<double>"),
                #            cpp_ast.Call(cpp_ast.CName(inner_name+"->push_back"), [cpp_ast.CNumber(0.0)]),
                cpp_ast.Assign(cpp_ast.Value("double", "accum"), cpp_ast.CNumber(0.0)),
                cpp_ast.For("sum_i",
                            0,
                            cpp_ast.BinOp(cpp_ast.Call(str(name)+"->size", []), "-", cpp_ast.CNumber(1)),
                            1,
                            cpp_ast.Block(contents=[
                                cpp_ast.Assign(#cpp_ast.Subscript("(*"+inner_name+")", cpp_ast.CNumber(0)),
                                    cpp_ast.CName("accum"),
                                    cpp_ast.BinOp(cpp_ast.CName("accum"), #cpp_ast.Subscript("(*"+inner_name+")", cpp_ast.CNumber(0)),
                                                  "+",
                                                         cpp_ast.Subscript("(*"+str(name)+")", cpp_ast.CName("sum_i"))))])),
                cpp_ast.Call(cpp_ast.CName(inner_name+"->push_back"), [cpp_ast.CName("accum")]),
                cpp_ast.ReturnStatement(cpp_ast.CName(inner_name))
                
               ]
            
            return cpp_ast.Call(cpp_ast.FunctionBody(CppLambda("&", cpp_ast.Value("std::vector<double>*", ""), [cpp_ast.Value("std::vector<double>*", name)]),
                                                     cpp_ast.Block(contents=block)),[self.visit(node.data)])


        

                                              

    def visit_Attribute(self, node):
        # at this point, attributes should refer to a real function that will also be
        # specialized.  Hence, just return the name.
        print "IN ATTRIBUTE"
        self.other_funcs += [node.attr.name]
        return self.visit(node.attr)

    def visit_Identifier(self, node):
        return node.name

    def visit_CuList(self, node):
        thisone = "tmp"+str(self.get_next_seq())
        self.intermediates += [cpp_ast.Initializer(cpp_ast.Value("double", "%s_[]" % thisone),
                                                   "{%s}" % (','.join([str(self.visit(x)) for x in node.elements]))),
                               cpp_ast.Assign(cpp_ast.Value("std::vector<double>", "*%s" % thisone),
                                              cpp_ast.Call("new std::vector<double>",
                                                           ["%s_" % thisone, "%s_+sizeof(%s_)/sizeof(%s_[0])" % (thisone, thisone, thisone)])),
#                               cpp_ast.Call("std::cout << \"SIZE: \" << %s->size" % thisone,[])
                              ]

        return cpp_ast.CName(thisone)


    def visit_Constant(self, node):
        return node.value

    def visit_BinaryOp(self, node):
        op_dict = {python_ast.Add:"+", python_ast.Sub:"-", python_ast.Mult:"*"}
        return cpp_ast.BinOp(self.visit(node.left), op_dict[type(node.op)], self.visit(node.right))
        

#    def visit(self, node):
#        print node
#        return self.generic_visit(node)