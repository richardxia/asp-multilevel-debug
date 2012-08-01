# this makefile compiles the libcppanalysis library for the instrumented c++ code to call during the execution

TARGET=libcppanalysis

SRCS=cpp_analysis.cpp
HEADERS=cpp_analysis.h

INCDIR= -I/usr/local/include -I/usr/lib/pymodules/python2.6/numpy/core/include/numpy -I/usr/include/python2.6 -I$(ASP_ROOT)/asp/analysis/glog/include
LIBDIR=-L$(ASP_ROOT)/asp/analysis/glog/lib -L/usr/local/lib
LIBS=-lglog -lboost_filesystem
FLAGS=-shared -g -Wall -fPIC
EXTRA_FLAGS=-fopenmp -O3 -msse3 -Wno-unknown-pragmas 

all: $(TARGET).so

$(TARGET).so: $(SRCS) $(HEADERS)
	g++ $(INCDIR) $(LIBDIR) $(LIBS) $(FLAGS) $(EXTRA_FLAGS) -o $@ $(SRCS)

clean:
	rm -rf $(TARGET).so
