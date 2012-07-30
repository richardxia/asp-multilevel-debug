import os
import shutil
import copy
#import codepy.cgen
import asp.codegen.cpp_ast as CppAst
import asp.codegen.ast_tools as CppAstTools

#############################################################################

from cpp_analysis_config import *

#############################################################################

def log(*args):
    if INSTRUMENT_CPP:
        s = ""
        for m in args:
            s += m
        print(s)

#############################################################################

def begin_test(kernel):
    global TEST_NAME, TEST_WORK_DIR
    TEST_NAME = kernel.__class__.__name__
    TEST_WORK_DIR = os.path.join(WORK_DIR, TEST_NAME)
    
    # delete test directory in work
    #if os.path.exists(TEST_WORK_DIR):
    #    shutil.rmtree(TEST_WORK_DIR)
    #os.mkdir(TEST_WORK_DIR)
    
    # clear log directory
    log_dir = os.path.join(ASP_ROOT, "asp/analysis/log")
    shutil.rmtree(log_dir)
    os.mkdir(log_dir)
    
    # delete the trace file
    if os.path.exists(TRACE_FILE):
        os.remove(TRACE_FILE)
    
    # disable Richard's tracing functionality. the trace is created by the analysis code
    if hasattr(kernel, "should_trace"):
        kernel.should_trace = False
    
#############################################################################

def save_variants_to_file(variants, variant_names, message, filename = "variants.cpp"):
    filename = os.path.join(TEST_WORK_DIR, filename)
    
    with open(filename, 'w') as f:
        for (variant, name) in zip(variants, variant_names):
            if name == "kernel_unroll_4":
                f.write("// %s\n" % message)
                #f.write("// Test: %s, Variant: %s\n" % (TEST_NAME, name))
                f.write(str(variant))
                f.write("\n\n")

#############################################################################

def cpp_instrument_for_analysis(kernel, variants, variant_names, mod):
    assert isinstance(variants, list)
    assert isinstance(variant_names, list)
    assert 0 < len(variants) == len(variant_names)
        
    if not INSTRUMENT_CPP:
        return variants
    
    if hasattr(kernel, "verify_log") and kernel.verify_log is True:
        return variants
    
    begin_test(kernel)
   
    add_libraries(mod)
    
    # print uninstrumented variants to a file
    save_variants_to_file(variants, variant_names, "CODE BEFORE INSTRUMENTED FOR ACTIVE TESTING", "variants_uninst.cpp")
    
    # do instrument the variants
    inst_variants = []
    for (variant, name) in zip(variants, variant_names):
        #log("Instrumenting variant %s..." % name)
        inst_variant = NodeInstrumentor(name).visit(variant)
        inst_variants.append(inst_variant)

    #log("\n*************** Instrumented %s for parallelization checks" % TEST_NAME)

    # print instrumented variants to a file
    save_variants_to_file(inst_variants, variant_names, "CODE INSTRUMENTED FOR ACTIVE TESTING", "variants_inst.cpp")
    
    #log("Done with instrumenting. Variants saved to %s\n" % TEST_WORK_DIR)
    
    return inst_variants

#############################################################################
  
# add instrumentation library
def add_libraries(mod):
    mod.add_library("cpp_analysis", 
                    include_dirs=[os.path.join(ASP_ROOT, "asp/analysis")], 
                    library_dirs=[os.path.join(ASP_ROOT, "asp/analysis")], 
                    libraries=["cppanalysis"], backend="c++")
    mod.add_header("cpp_analysis.h", backend="c++")
    
    mod.add_library("glog", 
                    include_dirs=[os.path.join(ASP_ROOT, "asp/analysis/glog/include")], 
                    library_dirs=[os.path.join(ASP_ROOT, "asp/analysis/glog/lib")], 
                    libraries=["glog"], backend="c++")
    mod.add_header("glog/logging.h", backend="c++")
    
    mod.add_to_preamble([CppAst.Statement("%s* %s" % (CPP_TX_CLASS, CPP_TX_OBJECT)),
                         CppAst.Pragma("omp threadprivate(%s)" % CPP_TX_OBJECT)])
    
#############################################################################
  
# Helper functions to create C/C++ AST nodes

# (void*)...
def CastToVoidPtr(node):
    return CppAst.TypeCast(CppAst.Pointer(CppAst.Value('void', '')), node)

# (double*)...
def CastToDoublePtr(node):
    return CppAst.TypeCast(CppAst.Pointer(CppAst.Value('double', '')), node)

# printf("message");
def CallPrintf(message):
    return CppAst.Call(CppAst.CName("printf"), [CppAst.String("%s" % message)])

# _LOG_ << ....
def CallLog(*message):
    stmt = "_LOG_ "
    for m in list(message):
        stmt += " << \"%s\"" % m
    return CppAst.Statement(stmt)

def ExtractTypenameFromDecl(node):
    assert isinstance(node, CppAst.Pointer) or isinstance(node, CppAst.Value), "Unexpected AST type: %s" % str(type(node))
    pointer = False
    if isinstance(node, CppAst.Pointer):
        node = node.subdecl
        pointer = True
    assert isinstance(node, CppAst.Value)
    return node.typename + ("*" if pointer else "")

def ExtractNameFromDecl(node):
    assert isinstance(node, CppAst.Pointer) or isinstance(node, CppAst.Value), "Unexpected AST type: %s" % str(type(node))
    if isinstance(node, CppAst.Pointer):
        node = node.subdecl
    assert isinstance(node, CppAst.Value)
    return node.name
    
#############################################################################

class State:
    NON = 0
    LHS = 1
    RHS = 2

#############################################################################
#############################################################################
        
# interface to create and call methods of C++ asp::Access and asp::AspEventHandler classes
class AspEventHandler:

    def __init__(self, variant_name):
        self.variant_name = variant_name
        
    ############################################################

    # define an instance (stack allocated)
    def Define(self):
        assert TEST_NAME is not None, "Test name is not specified (None); begin_test(kernel) must be called during the test setup!"
        return CppAst.Statement("%s %s(\"%s\", \"%s\")" % (CPP_HANDLER_CLASS, CPP_HANDLER_OBJECT, TEST_NAME, self.variant_name))
    
    ############################################################
    
    def NewAccess(self, node, method):
        if isinstance(node, CppAst.Subscript):
            return CppAst.FunctionCall(
                         CppAst.CName(method),
                         params=[CppAst.CName(CPP_TX_OBJECT),
                                 node.value,
                                 CppAst.String(str(node.value)),
                                 node.index,
                                 CppAst.String(str(node.index))])
        elif isinstance(node, CppAst.CName):
            return CppAst.FunctionCall(
                         CppAst.CName(method), 
                         params=[CppAst.CName(CPP_TX_OBJECT),
                                 CppAst.CName("&%s" % node.name),
                                 CppAst.String(str(node.name))])
        assert False, "Unexpected AST node type: %s expression: %s" % (type(node), str(node))
        
    ############################################################
    
    def NewReadAccess(self, node):
        return self.NewAccess(node, CPP_NEW_READ_ACCESS_OBJECT)
    
    ############################################################
    
    def NewWriteAccess(self, node):
        return self.NewAccess(node, CPP_NEW_WRITE_ACCESS_OBJECT)
    
    ############################################################
    
    def OnRead(self, node):
        assert not isinstance(node, CppAst.FunctionCall)
        node = self.NewReadAccess(node)
        if isinstance(node, CppAst.FunctionCall):
            return CppAst.FunctionCall(
                         CppAst.CName(CPP_HANDLER_OBJECT + "." + CPP_HANDLER_ON_READ),
                         params=[node])
        return node
    
    ############################################################
    
    def OnWrite(self, lnode, rnode):
        assert not isinstance(lnode, CppAst.FunctionCall)
        lnode = self.NewWriteAccess(lnode)
        if isinstance(lnode, CppAst.FunctionCall):
            return CppAst.FunctionCall(
                         CppAst.CName(CPP_HANDLER_OBJECT + "." + CPP_HANDLER_ON_WRITE),
                         params=[lnode, rnode])
        return CppAst.Assign(lnode, rnode)
    
    ############################################################
    
    def OnTransaction(self, loopvar):
        stmts_before = []
        stmts_after = []
        
        stmts_before.append(self.BeginTransaction(loopvar))
        
        stmts_after.append(self.EndTransaction())
        
        return (stmts_before, stmts_after)
    
    ############################################################
    
    def BeginTransaction(self, loopvar):
        return CppAst.Assign(
                     CppAst.CName(CPP_TX_OBJECT),
                     CppAst.FunctionCall(
                             CppAst.CName(CPP_HANDLER_OBJECT + "." + CPP_HANDLER_BEGIN_TX),
                             params=[CppAst.CName(loopvar)]))
        
    def EndTransaction(self):
        return CppAst.FunctionCall(
                     CppAst.CName(CPP_HANDLER_OBJECT + "." + CPP_HANDLER_END_TX),
                     params=[CppAst.CName(CPP_TX_OBJECT)])
     
    ############################################################   
    def EnterForLoop(self, loopvar, is_parallel = False):
        return CppAst.FunctionCall(
                     CppAst.CName(CPP_HANDLER_OBJECT + "." + CPP_HANDLER_ENTER_FORLOOP),
                     params=[CppAst.String(loopvar),
                             CppAst.CName("true" if is_parallel else "false")])
        
    def IterForLoop(self, loopindex):
        return CppAst.FunctionCall(
                     CppAst.CName(CPP_HANDLER_OBJECT + "." + CPP_HANDLER_ITER_FORLOOP),
                     params=[CppAst.CNumber(loopindex)])
    
    def ExitForLoop(self):
        return CppAst.FunctionCall(
                     CppAst.CName(CPP_HANDLER_OBJECT + "." + CPP_HANDLER_EXIT_FORLOOP),
                     params=[])
        
#############################################################################
#############################################################################
    
class NodeInstrumentor(CppAstTools.NodeTransformer):
    
    def __init__(self, variant_name):
        self.event_handler = AspEventHandler(variant_name)
        self.state = State.NON
        self.stmts_before = []
        self.stmts_after = []
        self.scopes = []
        self.in_parallel_for = False
        self.is_parallel_for = False
    
    ############################################################
    
    def visit_FunctionBody(self, node):
        # update scope
        self.enter_scope(node.fdecl.arg_decls)
        node.body = self.visit(node.body)
        self.exit_scope()
        
        # _cpp_transaction_ = NULL;
        node.body.contents.insert(0, CppAst.Assign(CppAst.CName(CPP_TX_OBJECT), CppAst.CName(NULL)))
        # asp::AspEventHandler _cpp_event_handler_(variant_name); 
        node.body.contents.insert(0, self.event_handler.Define())
        
        return node
    
    ############################################################
    
    def visit_Block(self, node):
        assert isinstance(node.contents, list)
        
        pending = []
        contents = []
        for x in node.contents:
            # reset the before and after lists
            self.stmts_before = []
            self.stmts_after = []
            
            # visit the statement
            x = self.visit(x)
            
            # we should insert pragmas exactly before the next statement
            if isinstance(x, CppAst.Pragma):
                pending.extend(self.stmts_before)
                pending.append(x) # <<<
                pending.extend(self.stmts_after)
            else:
                contents.extend(self.stmts_before)
                # insert any pending pragmas and empty pending
                contents.extend(pending)
                pending = []
                contents.append(x)
                contents.extend(self.stmts_after)
        
        node.contents = contents
        
        node.contents.insert(0, CallLog("Starting block"))
        node.contents.append(CallLog("Ending block"))
        
        return node
    
    ############################################################
    
    def visit_Assign(self, node):
        tmp_state = self.state
        
        self.state = State.RHS
        node.rvalue = self.visit(node.rvalue)
        
        self.state = State.LHS
        node.lvalue = self.visit(node.lvalue)
        
        if self.in_parallel_for:
            if isinstance(node.lvalue, CppAst.Pointer) or isinstance(node.lvalue, CppAst.Value):
                # define the variable before writing to it
                self.stmts_before.append(copy.deepcopy(node.lvalue))
                node.lvalue = CppAst.CName(ExtractNameFromDecl(node.lvalue))
            
            node = self.event_handler.OnWrite(node.lvalue, node.rvalue)
        
        self.state = tmp_state
        
        return node
        
    ############################################################
    
    def visit_Subscript(self, node):
#        if isinstance(node.value, str):
#            node.value = self.visit(CppAst.CName(node.value))
#        else:
#            node.value = self.visit(node.value)
            
#        tmp_state = self.state
#        self.state = State.RHS
#        node.index = self.visit(node.index)
#        self.state = tmp_state
        
        if self.in_parallel_for:
            if self.state is State.RHS:
                return self.event_handler.OnRead(node)
        
        return node
    
    ############################################################
    
    def visit_CName(self, node):
        if self.in_parallel_for:
            if self.in_scope(node.name):
                if self.state is State.RHS:
                    return self.event_handler.OnRead(node)
        
        return node
    
    ############################################################
    
    def visit_Initializer(self, node):
        assert isinstance(node.vdecl, CppAst.Value) or isinstance(node.vdecl, CppAst.Pointer)
        self.update_scope([node.vdecl])
        
        return self.visit(node)
    
    ############################################################
    
    def visit_Pragma(self, node):
        if node.value.startswith("omp parallel for"):
            assert not self.is_parallel_for and not self.in_parallel_for
            self.is_parallel_for = True
        
        return node
    
    ############################################################
    
    def visit_For(self, node):
        self.enter_scope([CppAst.Value("int", node.loopvar)])
        
        node.initial = self.visit(node.initial)
        node.end = self.visit(node.end)
        node.increment = self.visit(node.increment)
        
        node.set_underlying_for()
        
        ##############################
        if self.is_parallel_for:
            assert not self.in_parallel_for
            self.in_parallel_for = True
            self.is_parallel_for = False
            node.body = self.visit(node.body)
            self.in_parallel_for = False
            self.is_parallel_for = True
        else:
            node.body = self.visit(node.body)
            
        ##############################
        if not self.in_parallel_for:
            self.stmts_before.append(self.event_handler.EnterForLoop(node.loopvar, self.is_parallel_for))
            self.stmts_after.append(self.event_handler.ExitForLoop())
            if not self.is_parallel_for:
                node.body.contents.insert(0, self.event_handler.IterForLoop(node.loopvar))
        
        ##############################
        # check if parallel for, if so do more instrumentation
        if self.is_parallel_for:
            node = self.visit_Parallel_For(node)
            self.is_parallel_for = False
        
        self.exit_scope()
        
        return node
    
    ############################################################
    
    def visit_Parallel_For(self, node):
        # get the index variable name
        loopvar = node.loopvar
        
        body = node.body
        # create a thread local transaction object in the iteration
        # this returns the instrumentation to be inserted at the beginning and end of each iteration
        (stmts_before, stmts_after) = self.event_handler.OnTransaction(loopvar)
        # add instrumentation at the beginning of the body (iteration)
        stmts_before.extend(body.contents)
        stmts_before.extend(stmts_after)
        body.contents = stmts_before
        
        return node
    
    ############################################################
    
    # Helper functions to keep track of the current variable scope
    # self.scopes is a stack of scopes
    # each scope in this stack is a map from variable name (str) to type name (str)
    # when a new block/function begins, we push a new scope and pop it when the block ends.
    # local variable definitions do not push a new scope but only update the top scope  
    
    def in_scope(self, name):
        for scope in reversed(self.scopes):
            if name in scope:
                return True
        return False
    
    def update_scope(self, decl_list = []):
        scope = self.exit_scope()
        for decl in decl_list:
            assert isinstance(decl, CppAst.Value) or isinstance(decl, CppAst.Pointer)
            scope[ExtractNameFromDecl(decl)] = ExtractTypenameFromDecl(decl) 
        self.enter_scope(scope)
        
    def enter_scope(self, decl_list = []):
        scope = {}
        for decl in decl_list:
            assert isinstance(decl, CppAst.Value) or isinstance(decl, CppAst.Pointer)
            scope[ExtractNameFromDecl(decl)] = ExtractTypenameFromDecl(decl) 
        self.scopes.append(scope)
    
    def exit_scope(self):
        assert len(self.scopes) > 0
        return self.scopes.pop()
            
#############################################################################
#############################################################################

def cpp_hoist_index_variable_transformation(variants, variant_names):
    
    if not INSERT_BUG:
        return variants
        
    # print correct variants to a file
    save_variants_to_file(variants, variant_names, "CORRECT C++ CODE BEFORE OPTIMIZATION", "variants.cpp")
        
    inst_variants = []
    for variant in variants:
        inst_variant = BugInstrumentor().visit(variant)
        inst_variants.append(inst_variant)
        
    # print buggy variants to a file
    save_variants_to_file(inst_variants, variant_names, "BUGGY C++ CODE AFTER OPTIMIZATION", "variants_buggy.cpp")
        
    return inst_variants

#############################################################################

class BugInstrumentor(CppAstTools.NodeTransformer):
    
    ############################################################
    class IntDefFounder(CppAstTools.NodeTransformer):
        def __init__(self):
            self.defs = []
        
        def GetAndRemoveDefs(self, block):
            self.defs = []
            block = self.visit(block)
            return (block, self.defs)
        
        def visit_Block(self, node):
            contents = []
            for x in node.contents:
                try:
                    x = self.visit(x)
                except: pass
                if isinstance(x, CppAst.Value) and x.typename == "int":
                    self.defs.append(x)
                else:
                    contents.append(x)
            node.contents = contents
            return node
        
    ############################################################
    
    def visit_Block(self, node):
        i = 0
        while i < len(node.contents):
            x = node.contents[i]
            try:
                x = self.visit(x)
            except: pass
            node.contents[i] = x
            
            if isinstance(x, CppAst.Pragma):
                if x.value.startswith("omp parallel for"):
                    pf = node.contents[i+1]
                    assert isinstance(pf, CppAst.For), "A For-Loop is expected after omp parallel for"
                    
                    if BUG_HOIST_LOOPINDEX:
                        # hoist out the index var as shared
                        loopvar = pf.loopvar
                        node.contents.insert(i, CppAst.Value("int", loopvar))
                        pf.set_underlying_for(initialize=False) 
                        if BUG_MAKE_HOISTED_VARS_SHARED:
                            x.value += " shared(%s)" % loopvar
                        
                    if BUG_HOIST_LOCALINTS:                
                        # hoist inner integers as shared
                        (pf.body, defs) = BugInstrumentor.IntDefFounder().GetAndRemoveDefs(pf.body)
                        for v in defs:
                            assert isinstance(v, CppAst.Value)
                            node.contents.insert(i, v)
                            if BUG_MAKE_HOISTED_VARS_SHARED:
                                x.value += " shared(%s)" % v.name
            i += 1
        return node
        
#############################################################################
#############################################################################

def cpp_parallel_for_loop_transformation(variants, variant_names):
    
    if not INSERT_PARLOOP:
        return variants
        
    inst_variants = []
    for variant in variants:
        inst_variant = ParallelLoopInstrumentor().insert_parallel_loop(variant)
        inst_variants.append(inst_variant)
        
    # print uninstrumented variants to a file
    save_variants_to_file(inst_variants, variant_names, "C++ CODE GENERATED BY SEJITS", "variants.cpp")
        
    return inst_variants
    
#############################################################################

class ParallelLoopInstrumentor(CppAstTools.NodeTransformer):
    
    ############################################################
    class ParallelLoopChecker(CppAstTools.NodeVisitor):
        def __init__(self):
            self.has_parallel_loop_ = False
        
        def has_parallel_loop(self, node):
            self.visit(node)
            return self.has_parallel_loop_
        
        def visit_Pragma(self, node):
            if node.value.startswith("omp parallel for"):
                self.has_parallel_loop_ = True
    
    ############################################################
    
    def insert_parallel_loop(self, node):
        if not ParallelLoopInstrumentor.ParallelLoopChecker().has_parallel_loop(node):
            self.done = False
            node = self.visit(node)
            assert self.done
        return node
            
    ############################################################
    
    def visit_Block(self, node):
        if self.done: return node
        
        i = 0
        while i < len(node.contents):
            x = node.contents[i]
            if isinstance(x, CppAst.For):
                node.contents.insert(i, CppAst.Pragma("omp parallel for"))
                self.done = True
                i += 1
            try:
                x = self.visit(x)
            except: pass
            node.contents[i] = x
            
            i += 1
            
        return node    
    
#############################################################################
#############################################################################

# this is not used for now, but a possible bug
class BugInstrumentor2(CppAstTools.NodeTransformer):
    
    def visit_Block(self, node):
        i = 0
        while i < len(node.contents):
            x = node.contents[i]
            try:
                x = self.visit(x)
            except: pass
            node.contents[i] = x
            
            if isinstance(x, CppAst.Pragma):
                if x.value.startswith("omp parallel for"):
                    x.value += " default(shared)"
                    pf = node.contents[i+1]
                    node.contents[i+1] = CppAst.Block([CppAst.Pragma("omp for nowait"), pf])
                    
                    if BUG_HOIST_LOOPINDEX:
                        # hoist out the index var as shared
                        loopvar = pf.loopvar
                        node.contents.insert(i, CppAst.Value("int", loopvar))
                        pf.set_underlying_for(initialize=False)  
                        if BUG_MAKE_HOISTED_VARS_SHARED:
                            x.value += " shared(%s)" % loopvar
                    
                    if BUG_HOIST_LOCALINTS:
                        # hoist inner integers as shared
                        (pf.body, defs) = BugInstrumentor.IntDefFounder().GetAndRemoveDefs(pf.body)
                        for v in defs:
                            assert isinstance(v, CppAst.Value)
                            node.contents.insert(i, v)
                            if BUG_MAKE_HOISTED_VARS_SHARED:
                                x.value += " shared(%s)" % v.name
            i += 1
        return node

#############################################################################
#############################################################################

