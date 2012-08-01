from tinycu_sm import *
from tinycu_frontend import *
from tinycu_converter import *
from tinycu_optimizer import *
from asp.jit import asp_module

# (elmas): for the c++ parallelism analysis
from asp.analysis.cpp_instrument import *

class TinyCu(object):
    def __init__(self):
        # create Asp module to hold functions
        self.mod = asp_module.ASPModule(specializer="tinycu")
        
        # add headers/compile flags
        self.mod.add_header("vector")
        self.mod.add_header("iostream")
        self.mod.backends["c++"].toolchain.cflags += ["-std=c++0x", "-fopenmp"]

        # create SM
        import ast, inspect
        sm = TinyCuFrontend().parse(ast.parse(inspect.getsource(self.__call__).lstrip()), "std::vector<double>*")
        
        # get number of arguments to __call__
        num_arguments = len(sm.inputs)-1

        # translate sm
        cpp_ast, new_funcs = TinyCuConverter().convert(sm)
        print str(cpp_ast)
        
        print "New funcs: ", new_funcs


        for x in new_funcs:
            sm = TinyCuFrontend().parse(ast.parse(inspect.getsource(getattr(self,x)).lstrip()), locals=self.__dict__)
            self.mod.add_function(x, TinyCuConverter().convert(sm)[0])

        # optimize just the main function for now
        cpp_ast2 = TinyCuOptimizer().optimize(cpp_ast)
#        cpp_ast2 = cpp_ast
        print str(cpp_ast2)

        ###########################################################################
        
        # C++ LEVEL INSTRUMENTATION FOR CHECKING PARALLELISM ERRORS
        # Note: takes lists of variants/names and returns a new list of the given length
        [cpp_ast2] = cpp_instrument_for_analysis(self, [cpp_ast2], ["cpp_ast2"], self.mod)
        print str(cpp_ast2)
        
        ###########################################################################

        self.mod.backends["c++"].module.add_to_module([cpp_ast2])

        self.mod.add_function("returner", self.gen_returner(num_arguments))
        print self.mod.generate()
        

    def gen_returner(self, num_args):
        argnames = ["tmp"+str(i) for i in range(num_args)]
        import asp.codegen.templating.template as template
        return template.Template("""
        boost::python::list returner(${','.join(["boost::python::list "+i for i in argnames])}) {
          boost::python::list *ret = new boost::python::list;

          // unpack into vectors
          % for x in argnames:

          std::vector<double> ${x}_;
          for (int i=0; i<boost::python::len(${x}); i++)
              ${x}_.push_back(boost::python::extract<double>(${x}[i]));

          % endfor

          std::vector<double>* foo = __call__(${','.join(["&"+x+"_" for x in argnames])});
          std::cout << "\\nSIZEAGAIN: " << foo->size();
//          BOOST_FOREACH(double it, foo) ret->append(it);
          for (int i=0; i<foo->size(); i++)
            ret->append((*foo)[i]);

          return *ret;

        }""", disable_unicode=True).render(argnames=argnames)

    def intercept_call(self, *args):
        return self.mod.returner(*args)