import os

assert "ASP_ROOT" in os.environ
ASP_ROOT = os.environ["ASP_ROOT"]
assert os.path.exists(ASP_ROOT)
WORK_DIR = (os.environ["WORK_DIR"] if "WORK_DIR" in os.environ else os.path.join(ASP_ROOT, "asp/analysis/work"))

TRACE_FILE = "/tmp/trace.txt"

#############################################################################

INSTRUMENT_CPP = (os.environ["INSTRUMENT_CPP"] == "1" if "INSTRUMENT_CPP" in os.environ else False)
INSERT_BUG = (os.environ["INSERT_BUG"] == "1" if "INSERT_BUG" in os.environ else False)
INSERT_PARLOOP = (os.environ["INSERT_PARLOOP"] == "1" if "INSERT_PARLOOP" in os.environ else False)

#############################################################################

BUG_HOIST_LOOPINDEX = False
BUG_HOIST_LOCALINTS = True
BUG_REMOVE_REDUCTION_PRAGMA = True
BUG_MAKE_HOISTED_VARS_SHARED = False

#############################################################################

# these two are updated by begin_test() and end_test()
TEST_NAME = (os.environ["TEST_NAME"] if "TEST_NAME" in os.environ else None)
TEST_WORK_DIR = os.path.join(WORK_DIR, TEST_NAME) if (TEST_NAME is not None) else None 

#############################################################################

CPP_ACCESS_CLASS = "asp::Access"
CPP_NEW_READ_ACCESS_OBJECT = "asp::NewReadAccess" 
CPP_NEW_WRITE_ACCESS_OBJECT = "asp::NewWriteAccess" 

CPP_HANDLER_CLASS = "asp::AspEventHandler"
CPP_HANDLER_OBJECT = "_cpp_event_handler_"

CPP_HANDLER_ON_READ = "OnRead"
CPP_HANDLER_ON_WRITE = "OnWrite"

CPP_HANDLER_BEGIN_TX = "BeginTransaction"
CPP_HANDLER_END_TX = "EndTransaction"

CPP_HANDLER_ENTER_FORLOOP = "EnterForLoop"
CPP_HANDLER_ITER_FORLOOP = "IterForLoop"
CPP_HANDLER_EXIT_FORLOOP = "ExitForLoop"

CPP_TX_CLASS = "asp::Transaction"
CPP_TX_OBJECT = "_cpp_transaction_"

NULL = "NULL"

#############################################################################