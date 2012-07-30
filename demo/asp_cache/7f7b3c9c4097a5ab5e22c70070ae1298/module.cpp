#include <boost/python.hpp>
#include "arrayobject.h"
#include "cpp_analysis.h"
#include "glog/logging.h"
asp::Transaction* _cpp_transaction_;
#pragma omp threadprivate(_cpp_transaction_)

namespace private_namespace_e8569f26bd3caeac490e33f977da31b4
{
  void kernel(PyObject *in_grid, PyObject *out_grid)
  {
    asp::AspEventHandler _cpp_event_handler_("TestKernel", "kernel");
    _cpp_transaction_ = NULL;
    _LOG_  << "Starting block";
    #define _out_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    #define _in_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    npy_double *_my_out_grid = ((npy_double *)PyArray_DATA(out_grid));
    npy_double *_my_in_grid = ((npy_double *)PyArray_DATA(in_grid));
    _cpp_event_handler_.EnterForLoop("x1",true);
    #pragma omp parallel for
    for (int x1 = 1; (x1 <= 8); x1 = (x1 + 1))
    {
      _cpp_transaction_ = _cpp_event_handler_.BeginTransaction(x1);
      _LOG_  << "Starting block";
      #pragma ivdep
      for (int x2 = 1; (x2 <= 8); x2 = (x2 + 1))
      {
        _LOG_  << "Starting block";
        int x3;
        _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,&x3,"x3"),_out_grid_array_macro(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x1,"x1")), _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x2,"x2"))));
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), (x2 + -1)),"_in_grid_array_macro((x1 + 0), (x2 + -1))"))));
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + -1), (x2 + 0)),"_in_grid_array_macro((x1 + -1), (x2 + 0))"))));
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 1), (x2 + 0)),"_in_grid_array_macro((x1 + 1), (x2 + 0))"))));
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), (x2 + 1)),"_in_grid_array_macro((x1 + 0), (x2 + 1))"))));
        _LOG_  << "Ending block";
      }
      _LOG_  << "Ending block";
      _cpp_event_handler_.EndTransaction(_cpp_transaction_);
    }
    _cpp_event_handler_.ExitForLoop();
    _LOG_  << "Ending block";
  }
  void kernel_unroll_2(PyObject *in_grid, PyObject *out_grid)
  {
    asp::AspEventHandler _cpp_event_handler_("TestKernel", "kernel_unroll_2");
    _cpp_transaction_ = NULL;
    _LOG_  << "Starting block";
    #define min(_a,_b) (_a < _b ?  _a : _b)
    #define _out_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    #define _in_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    npy_double *_my_out_grid = ((npy_double *)PyArray_DATA(out_grid));
    npy_double *_my_in_grid = ((npy_double *)PyArray_DATA(in_grid));
    _cpp_event_handler_.EnterForLoop("x1",true);
    #pragma omp parallel for
    for (int x1 = 1; (x1 <= 8); x1 = (x1 + 1))
    {
      _cpp_transaction_ = _cpp_event_handler_.BeginTransaction(x1);
      _LOG_  << "Starting block";
      #pragma ivdep
       for (int x2 = 1; (x2 <= (8 - 1)); x2 = (x2 + (1 * 2)))
       {
         _LOG_  << "Starting block";
         int x3;
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,&x3,"x3"),_out_grid_array_macro(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x1,"x1")), _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x2,"x2"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), (x2 + -1)),"_in_grid_array_macro((x1 + 0), (x2 + -1))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + -1), (x2 + 0)),"_in_grid_array_macro((x1 + -1), (x2 + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 1), (x2 + 0)),"_in_grid_array_macro((x1 + 1), (x2 + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), (x2 + 1)),"_in_grid_array_macro((x1 + 0), (x2 + 1))"))));
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,&x3,"x3"),_out_grid_array_macro(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x1,"x1")), (_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x2,"x2")) + 1)));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 1) + -1)),"_in_grid_array_macro((x1 + 0), ((x2 + 1) + -1))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + -1), ((x2 + 1) + 0)),"_in_grid_array_macro((x1 + -1), ((x2 + 1) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 1), ((x2 + 1) + 0)),"_in_grid_array_macro((x1 + 1), ((x2 + 1) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 1) + 1)),"_in_grid_array_macro((x1 + 0), ((x2 + 1) + 1))"))));
         _LOG_  << "Ending block";
       }
      _LOG_  << "Ending block";
      _cpp_event_handler_.EndTransaction(_cpp_transaction_);
    }
    _cpp_event_handler_.ExitForLoop();
    _LOG_  << "Ending block";
  }
  void kernel_unroll_4(PyObject *in_grid, PyObject *out_grid)
  {
    asp::AspEventHandler _cpp_event_handler_("TestKernel", "kernel_unroll_4");
    _cpp_transaction_ = NULL;
    _LOG_  << "Starting block";
    #define min(_a,_b) (_a < _b ?  _a : _b)
    #define _out_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    #define _in_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    npy_double *_my_out_grid = ((npy_double *)PyArray_DATA(out_grid));
    npy_double *_my_in_grid = ((npy_double *)PyArray_DATA(in_grid));
    _cpp_event_handler_.EnterForLoop("x1",true);
    #pragma omp parallel for
    for (int x1 = 1; (x1 <= 8); x1 = (x1 + 1))
    {
      _cpp_transaction_ = _cpp_event_handler_.BeginTransaction(x1);
      _LOG_  << "Starting block";
      #pragma ivdep
       for (int x2 = 1; (x2 <= (8 - 3)); x2 = (x2 + (1 * 4)))
       {
         _LOG_  << "Starting block";
         int x3;
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,&x3,"x3"),_out_grid_array_macro(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x1,"x1")), _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x2,"x2"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), (x2 + -1)),"_in_grid_array_macro((x1 + 0), (x2 + -1))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + -1), (x2 + 0)),"_in_grid_array_macro((x1 + -1), (x2 + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 1), (x2 + 0)),"_in_grid_array_macro((x1 + 1), (x2 + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), (x2 + 1)),"_in_grid_array_macro((x1 + 0), (x2 + 1))"))));
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,&x3,"x3"),_out_grid_array_macro(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x1,"x1")), (_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x2,"x2")) + 1)));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 1) + -1)),"_in_grid_array_macro((x1 + 0), ((x2 + 1) + -1))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + -1), ((x2 + 1) + 0)),"_in_grid_array_macro((x1 + -1), ((x2 + 1) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 1), ((x2 + 1) + 0)),"_in_grid_array_macro((x1 + 1), ((x2 + 1) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 1) + 1)),"_in_grid_array_macro((x1 + 0), ((x2 + 1) + 1))"))));
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,&x3,"x3"),_out_grid_array_macro(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x1,"x1")), (_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x2,"x2")) + 2)));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 2) + -1)),"_in_grid_array_macro((x1 + 0), ((x2 + 2) + -1))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + -1), ((x2 + 2) + 0)),"_in_grid_array_macro((x1 + -1), ((x2 + 2) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 1), ((x2 + 2) + 0)),"_in_grid_array_macro((x1 + 1), ((x2 + 2) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 2) + 1)),"_in_grid_array_macro((x1 + 0), ((x2 + 2) + 1))"))));
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,&x3,"x3"),_out_grid_array_macro(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x1,"x1")), (_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x2,"x2")) + 3)));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 3) + -1)),"_in_grid_array_macro((x1 + 0), ((x2 + 3) + -1))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + -1), ((x2 + 3) + 0)),"_in_grid_array_macro((x1 + -1), ((x2 + 3) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 1), ((x2 + 3) + 0)),"_in_grid_array_macro((x1 + 1), ((x2 + 3) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 3) + 1)),"_in_grid_array_macro((x1 + 0), ((x2 + 3) + 1))"))));
         _LOG_  << "Ending block";
       }
      _LOG_  << "Ending block";
      _cpp_event_handler_.EndTransaction(_cpp_transaction_);
    }
    _cpp_event_handler_.ExitForLoop();
    _LOG_  << "Ending block";
  }
  void kernel_unroll_8(PyObject *in_grid, PyObject *out_grid)
  {
    asp::AspEventHandler _cpp_event_handler_("TestKernel", "kernel_unroll_8");
    _cpp_transaction_ = NULL;
    _LOG_  << "Starting block";
    #define min(_a,_b) (_a < _b ?  _a : _b)
    #define _out_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    #define _in_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    npy_double *_my_out_grid = ((npy_double *)PyArray_DATA(out_grid));
    npy_double *_my_in_grid = ((npy_double *)PyArray_DATA(in_grid));
    _cpp_event_handler_.EnterForLoop("x1",true);
    #pragma omp parallel for
    for (int x1 = 1; (x1 <= 8); x1 = (x1 + 1))
    {
      _cpp_transaction_ = _cpp_event_handler_.BeginTransaction(x1);
      _LOG_  << "Starting block";
      #pragma ivdep
       for (int x2 = 1; (x2 <= (8 - 7)); x2 = (x2 + (1 * 8)))
       {
         _LOG_  << "Starting block";
         int x3;
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,&x3,"x3"),_out_grid_array_macro(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x1,"x1")), _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x2,"x2"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), (x2 + -1)),"_in_grid_array_macro((x1 + 0), (x2 + -1))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + -1), (x2 + 0)),"_in_grid_array_macro((x1 + -1), (x2 + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 1), (x2 + 0)),"_in_grid_array_macro((x1 + 1), (x2 + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), (x2 + 1)),"_in_grid_array_macro((x1 + 0), (x2 + 1))"))));
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,&x3,"x3"),_out_grid_array_macro(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x1,"x1")), (_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x2,"x2")) + 1)));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 1) + -1)),"_in_grid_array_macro((x1 + 0), ((x2 + 1) + -1))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + -1), ((x2 + 1) + 0)),"_in_grid_array_macro((x1 + -1), ((x2 + 1) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 1), ((x2 + 1) + 0)),"_in_grid_array_macro((x1 + 1), ((x2 + 1) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 1) + 1)),"_in_grid_array_macro((x1 + 0), ((x2 + 1) + 1))"))));
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,&x3,"x3"),_out_grid_array_macro(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x1,"x1")), (_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x2,"x2")) + 2)));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 2) + -1)),"_in_grid_array_macro((x1 + 0), ((x2 + 2) + -1))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + -1), ((x2 + 2) + 0)),"_in_grid_array_macro((x1 + -1), ((x2 + 2) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 1), ((x2 + 2) + 0)),"_in_grid_array_macro((x1 + 1), ((x2 + 2) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 2) + 1)),"_in_grid_array_macro((x1 + 0), ((x2 + 2) + 1))"))));
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,&x3,"x3"),_out_grid_array_macro(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x1,"x1")), (_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x2,"x2")) + 3)));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 3) + -1)),"_in_grid_array_macro((x1 + 0), ((x2 + 3) + -1))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + -1), ((x2 + 3) + 0)),"_in_grid_array_macro((x1 + -1), ((x2 + 3) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 1), ((x2 + 3) + 0)),"_in_grid_array_macro((x1 + 1), ((x2 + 3) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 3) + 1)),"_in_grid_array_macro((x1 + 0), ((x2 + 3) + 1))"))));
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,&x3,"x3"),_out_grid_array_macro(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x1,"x1")), (_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x2,"x2")) + 4)));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 4) + -1)),"_in_grid_array_macro((x1 + 0), ((x2 + 4) + -1))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + -1), ((x2 + 4) + 0)),"_in_grid_array_macro((x1 + -1), ((x2 + 4) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 1), ((x2 + 4) + 0)),"_in_grid_array_macro((x1 + 1), ((x2 + 4) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 4) + 1)),"_in_grid_array_macro((x1 + 0), ((x2 + 4) + 1))"))));
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,&x3,"x3"),_out_grid_array_macro(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x1,"x1")), (_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x2,"x2")) + 5)));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 5) + -1)),"_in_grid_array_macro((x1 + 0), ((x2 + 5) + -1))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + -1), ((x2 + 5) + 0)),"_in_grid_array_macro((x1 + -1), ((x2 + 5) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 1), ((x2 + 5) + 0)),"_in_grid_array_macro((x1 + 1), ((x2 + 5) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 5) + 1)),"_in_grid_array_macro((x1 + 0), ((x2 + 5) + 1))"))));
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,&x3,"x3"),_out_grid_array_macro(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x1,"x1")), (_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x2,"x2")) + 6)));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 6) + -1)),"_in_grid_array_macro((x1 + 0), ((x2 + 6) + -1))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + -1), ((x2 + 6) + 0)),"_in_grid_array_macro((x1 + -1), ((x2 + 6) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 1), ((x2 + 6) + 0)),"_in_grid_array_macro((x1 + 1), ((x2 + 6) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 6) + 1)),"_in_grid_array_macro((x1 + 0), ((x2 + 6) + 1))"))));
         _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,&x3,"x3"),_out_grid_array_macro(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x1,"x1")), (_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,&x2,"x2")) + 7)));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 7) + -1)),"_in_grid_array_macro((x1 + 0), ((x2 + 7) + -1))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + -1), ((x2 + 7) + 0)),"_in_grid_array_macro((x1 + -1), ((x2 + 7) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 1), ((x2 + 7) + 0)),"_in_grid_array_macro((x1 + 1), ((x2 + 7) + 0))"))));
          _cpp_event_handler_.OnWrite(asp::NewWriteAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3"),(_cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_out_grid,"_my_out_grid",x3,"x3")) + _cpp_event_handler_.OnRead(asp::NewReadAccess(_cpp_transaction_,_my_in_grid,"_my_in_grid",_in_grid_array_macro((x1 + 0), ((x2 + 7) + 1)),"_in_grid_array_macro((x1 + 0), ((x2 + 7) + 1))"))));
         _LOG_  << "Ending block";
       }
      _LOG_  << "Ending block";
      _cpp_event_handler_.EndTransaction(_cpp_transaction_);
    }
    _cpp_event_handler_.ExitForLoop();
    _LOG_  << "Ending block";
  }
}

using namespace private_namespace_e8569f26bd3caeac490e33f977da31b4;

BOOST_PYTHON_MODULE(module)
{
  import_array();;
  boost::python::def("kernel", &kernel);
  boost::python::def("kernel_unroll_2", &kernel_unroll_2);
  boost::python::def("kernel_unroll_4", &kernel_unroll_4);
  boost::python::def("kernel_unroll_8", &kernel_unroll_8);
}
