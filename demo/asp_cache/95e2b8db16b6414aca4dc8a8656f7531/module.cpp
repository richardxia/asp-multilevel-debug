#include <boost/python.hpp>
#include "arrayobject.h"
#include <fstream>

namespace private_namespace_364e5f857567297f0ee5179fa9776269
{
  void kernel(PyObject *in_grid, PyObject *out_grid)
  {
     std::ofstream _asp_log_file;
     _asp_log_file.open("/tmp/trace.txt");
    #define _out_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    #define _in_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    npy_double *_my_out_grid = ((npy_double *)PyArray_DATA(out_grid));
    npy_double *_my_in_grid = ((npy_double *)PyArray_DATA(in_grid));
    #pragma omp parallel for
    for (int x1 = 1; (x1 <= 8); x1 = (x1 + 1))
    {
      #pragma ivdep
      for (int x2 = 1; (x2 <= 8); x2 = (x2 + 1))
      {
        int x3;
        x3 = _out_grid_array_macro(x1, x2);
         _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), (x2 + -1))]);
         _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), (x2 + -1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
         _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + -1), (x2 + 0))]);
         _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + -1), (x2 + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
         _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 1), (x2 + 0))]);
         _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 1), (x2 + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
         _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), (x2 + 1))]);
         _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), (x2 + 1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
      }
    }
    _asp_log_file.close();
  }
  void kernel_unroll_2(PyObject *in_grid, PyObject *out_grid)
  {
    #define min(_a,_b) (_a < _b ?  _a : _b)
     std::ofstream _asp_log_file;
     _asp_log_file.open("/tmp/trace.txt");
    #define _out_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    #define _in_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    npy_double *_my_out_grid = ((npy_double *)PyArray_DATA(out_grid));
    npy_double *_my_in_grid = ((npy_double *)PyArray_DATA(in_grid));
    #pragma omp parallel for
    for (int x1 = 1; (x1 <= 8); x1 = (x1 + 1))
    {
      #pragma ivdep
       for (int x2 = 1; (x2 <= (8 - 1)); x2 = (x2 + (1 * 2)))
       {
         int x3;
         x3 = _out_grid_array_macro(x1, x2);
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), (x2 + -1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), (x2 + -1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + -1), (x2 + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + -1), (x2 + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 1), (x2 + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 1), (x2 + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), (x2 + 1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), (x2 + 1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
         x3 = _out_grid_array_macro(x1, (x2 + 1));
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 1) + -1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 1) + -1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + -1), ((x2 + 1) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + -1), ((x2 + 1) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 1), ((x2 + 1) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 1), ((x2 + 1) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 1) + 1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 1) + 1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
       }
    }
    _asp_log_file.close();
  }
  void kernel_unroll_4(PyObject *in_grid, PyObject *out_grid)
  {
    #define min(_a,_b) (_a < _b ?  _a : _b)
     std::ofstream _asp_log_file;
     _asp_log_file.open("/tmp/trace.txt");
    #define _out_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    #define _in_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    npy_double *_my_out_grid = ((npy_double *)PyArray_DATA(out_grid));
    npy_double *_my_in_grid = ((npy_double *)PyArray_DATA(in_grid));
    #pragma omp parallel for
    for (int x1 = 1; (x1 <= 8); x1 = (x1 + 1))
    {
      #pragma ivdep
       for (int x2 = 1; (x2 <= (8 - 3)); x2 = (x2 + (1 * 4)))
       {
         int x3;
         x3 = _out_grid_array_macro(x1, x2);
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), (x2 + -1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), (x2 + -1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + -1), (x2 + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + -1), (x2 + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 1), (x2 + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 1), (x2 + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), (x2 + 1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), (x2 + 1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
         x3 = _out_grid_array_macro(x1, (x2 + 1));
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 1) + -1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 1) + -1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + -1), ((x2 + 1) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + -1), ((x2 + 1) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 1), ((x2 + 1) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 1), ((x2 + 1) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 1) + 1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 1) + 1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
         x3 = _out_grid_array_macro(x1, (x2 + 2));
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 2) + -1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 2) + -1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + -1), ((x2 + 2) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + -1), ((x2 + 2) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 1), ((x2 + 2) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 1), ((x2 + 2) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 2) + 1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 2) + 1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
         x3 = _out_grid_array_macro(x1, (x2 + 3));
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 3) + -1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 3) + -1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + -1), ((x2 + 3) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + -1), ((x2 + 3) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 1), ((x2 + 3) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 1), ((x2 + 3) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 3) + 1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 3) + 1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
       }
    }
    _asp_log_file.close();
  }
  void kernel_unroll_8(PyObject *in_grid, PyObject *out_grid)
  {
    #define min(_a,_b) (_a < _b ?  _a : _b)
     std::ofstream _asp_log_file;
     _asp_log_file.open("/tmp/trace.txt");
    #define _out_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    #define _in_grid_array_macro(_d0,_d1) (_d1+(_d0 * 10))
    npy_double *_my_out_grid = ((npy_double *)PyArray_DATA(out_grid));
    npy_double *_my_in_grid = ((npy_double *)PyArray_DATA(in_grid));
    #pragma omp parallel for
    for (int x1 = 1; (x1 <= 8); x1 = (x1 + 1))
    {
      #pragma ivdep
       for (int x2 = 1; (x2 <= (8 - 7)); x2 = (x2 + (1 * 8)))
       {
         int x3;
         x3 = _out_grid_array_macro(x1, x2);
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), (x2 + -1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), (x2 + -1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + -1), (x2 + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + -1), (x2 + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 1), (x2 + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 1), (x2 + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), (x2 + 1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), (x2 + 1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
         x3 = _out_grid_array_macro(x1, (x2 + 1));
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 1) + -1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 1) + -1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + -1), ((x2 + 1) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + -1), ((x2 + 1) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 1), ((x2 + 1) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 1), ((x2 + 1) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 1) + 1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 1) + 1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
         x3 = _out_grid_array_macro(x1, (x2 + 2));
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 2) + -1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 2) + -1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + -1), ((x2 + 2) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + -1), ((x2 + 2) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 1), ((x2 + 2) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 1), ((x2 + 2) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 2) + 1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 2) + 1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
         x3 = _out_grid_array_macro(x1, (x2 + 3));
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 3) + -1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 3) + -1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + -1), ((x2 + 3) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + -1), ((x2 + 3) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 1), ((x2 + 3) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 1), ((x2 + 3) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 3) + 1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 3) + 1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
         x3 = _out_grid_array_macro(x1, (x2 + 4));
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 4) + -1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 4) + -1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + -1), ((x2 + 4) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + -1), ((x2 + 4) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 1), ((x2 + 4) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 1), ((x2 + 4) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 4) + 1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 4) + 1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
         x3 = _out_grid_array_macro(x1, (x2 + 5));
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 5) + -1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 5) + -1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + -1), ((x2 + 5) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + -1), ((x2 + 5) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 1), ((x2 + 5) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 1), ((x2 + 5) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 5) + 1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 5) + 1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
         x3 = _out_grid_array_macro(x1, (x2 + 6));
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 6) + -1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 6) + -1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + -1), ((x2 + 6) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + -1), ((x2 + 6) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 1), ((x2 + 6) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 1), ((x2 + 6) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 6) + 1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 6) + 1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
         x3 = _out_grid_array_macro(x1, (x2 + 7));
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 7) + -1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 7) + -1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + -1), ((x2 + 7) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + -1), ((x2 + 7) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 1), ((x2 + 7) + 0))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 1), ((x2 + 7) + 0)) << ", " << _my_out_grid[x3] << ")" << std::endl;
          _my_out_grid[x3] = ((2 * _my_out_grid[x3]) + _my_in_grid[_in_grid_array_macro((x1 + 0), ((x2 + 7) + 1))]);
          _asp_log_file << "_asp_log_write(" << "out_grid" << ", " << x3 << ", " << _in_grid_array_macro((x1 + 0), ((x2 + 7) + 1)) << ", " << _my_out_grid[x3] << ")" << std::endl;
       }
    }
    _asp_log_file.close();
  }
}

using namespace private_namespace_364e5f857567297f0ee5179fa9776269;

BOOST_PYTHON_MODULE(module)
{
  import_array();;
  boost::python::def("kernel", &kernel);
  boost::python::def("kernel_unroll_2", &kernel_unroll_2);
  boost::python::def("kernel_unroll_4", &kernel_unroll_4);
  boost::python::def("kernel_unroll_8", &kernel_unroll_8);
}
