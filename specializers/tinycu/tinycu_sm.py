import asp.tree_grammar
import ast
import types

asp.tree_grammar.parse('''

# top level node
CuFunction(name=types.StringType, returntype=types.StringType, inputs=Identifier*, body=Expr)


Expr =  Constant
          | Identifier 
          | BinaryOp
          | Attribute
          | FunctionReturn
          | CuMap
          | CuSum
          | CuList

# a map of func applied to some data
CuMap(func=(Identifier | Attribute), data=(CuList | Identifier)*)

# a sum operation
CuSum(data = (CuList | Identifier | CuMap))

# a Python list that is turned into doubles or ints
CuList(elements=(Constant | Identifier)*)

Identifier(name=types.StringType)


Constant(value = types.IntType | types.FloatType)

BinaryOp(left=Expr, op=(ast.Add | ast.Sub | ast.Mult), right=Expr)

# this if for a.b
Attribute(value=Identifier, attr=Identifier)

FunctionReturn(value = Expr)
''', globals())
