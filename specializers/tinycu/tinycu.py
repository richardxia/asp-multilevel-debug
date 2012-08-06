from tinycu_sm import *
from tinycu_frontend import *
from tinycu_converter import *
from tinycu_optimizer import *
from asp.jit import asp_module
import asp.codegen.cpp_ast
from asp.verify import SpecializationError

# (elmas): for the c++ parallelism analysis
from asp.analysis.cpp_instrument import *

class TinyCu(object):
    def __init__(self, should_trace=False, pure_python=False):
        # create Asp module to hold functions
        self.mod = asp_module.ASPModule(specializer="tinycu")
        
        # add headers/compile flags
        self.mod.add_header("vector")
        self.mod.add_header("iostream")
        self.mod.backends["c++"].toolchain.cflags += ["-std=c++0x", "-fopenmp"]

        # multilevel debugging
        self.should_trace = should_trace
        self.pure_python = pure_python

        # create SM
        import ast, inspect
        sm = TinyCuFrontend(should_trace=self.should_trace).parse(ast.parse(inspect.getsource(self.__call__).lstrip()), "std::vector<double>*")
        
        # get number of arguments to __call__
        num_arguments = len(sm.inputs)-1

        # translate sm
        cpp_ast, new_funcs = TinyCuConverter(should_trace=self.should_trace).convert(sm)
        print str(cpp_ast)
        
        print "New funcs: ", new_funcs


        for x in new_funcs:
            sm = TinyCuFrontend(should_trace=self.should_trace).parse(ast.parse(inspect.getsource(getattr(self,x)).lstrip()), locals=self.__dict__)
            self.mod.add_function(x, TinyCuConverter(should_trace=self.should_trace).convert(sm)[0])

        # optimize just the main function for now
        cpp_ast2 = TinyCuOptimizer().optimize(cpp_ast)
#        cpp_ast2 = cpp_ast
        print str(cpp_ast2)

        ###########################################################################
        
        # "BUGGY" C++ LEVEL TRANSFORMATION (HOIST INT VARIABLES OUT OF FOR LOOP)
        [cpp_ast2] = cpp_buggy_parallelism_transformation(self, [cpp_ast2], ["cpp_ast2"])
        print str(cpp_ast2)
        
        # C++ LEVEL INSTRUMENTATION FOR CHECKING PARALLELISM ERRORS
        # Note: takes lists of variants/names and returns a new list of the given length
        [cpp_ast2] = cpp_instrument_for_analysis(self, [cpp_ast2], ["cpp_ast2"], self.mod)
        print str(cpp_ast2)
        
        ###########################################################################

        self.mod.backends["c++"].module.add_to_module([cpp_ast2])
        if self.should_trace:
            self.mod.backends['c++'].module.add_to_preamble([asp.codegen.cpp_ast.Include("fstream", True)])

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
        if self.pure_python:
            return self.__call__(*args)
        return self.mod.returner(*args)

    def replay(self, *args):
        self.instrument_reduce()
        return self.__call__(*args)

    def instrument_reduce(self):
        def instrumented_reduce(func, arr):
            results = arr[:]

            log = LogReader("/tmp/trace.txt")
            for line in log:
                x = int(line[0][1])
                y = int(line[1][1])
                z = int(line[2][1])

                results[z] = func(results[x], results[y])
                if float(line[2][2]) != results[z]:
                    raise SpecializationError, "Expected index %s to have value %s, got %s" % (z, line[2][2], results[z])
            return results[0:1]

        TinyCuReducer.reduce = staticmethod(instrumented_reduce)

class LogReader(object):
    def __init__(self, logfile):
        self.log = []
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
        self.log.append(args)

    def __iter__(self):
        count = 0
        returner = []
        for line in self.log:
            count += 1
            returner.append(line)
            if count % 3 == 0:
                yield returner
                count = 0
                returner = []

class TinyCuReducer(object):
    @staticmethod
    def indexes(size):
        for i in range(size-1):
            yield (0,i+1)

    @staticmethod
    def reduce(func, arr):
        results = arr[:]
        for x,y,z in TinyCuReducer.indexes(len(arr)):
            results[z] = func(results[x], results[y])
        return results[0:1]
