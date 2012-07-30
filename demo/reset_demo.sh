#!/bin/sh

# emacs colors
# 255 255 224
COLOR_TEST=LightYellow
# 178 223 238
COLOR_ASP=LightBlue2
# 211 211 211
COLOR_CPP=LightGray
# 255 255 255
COLOR_REPORT=white

export GITHUB_ASP_ROOT=/media/sf_ASP/github/asp

export ASP_ROOT=/home/vagrant/asp
export DEMO_ROOT=$ASP_ROOT/demo
export ANALYSIS_ROOT=$ASP_ROOT/asp/analysis
export STENCIL_ROOT=$ASP_ROOT/specializers/stencil

export PYTHONPATH=$ASP_ROOT:$ASP_ROOT/asp:$ASP_ROOT/asp/codegen:$ANALYSIS_ROOT:$STENCIL_ROOT:$PYTHONPATH
export LD_LIBRARY_PATH=$ANALYSIS_ROOT:$ANALYSIS_ROOT/glog/lib:$LD_LIBRARY_PATH
export GLOG_logtostderr=0
export GLOG_log_dir=$ANALYSIS_ROOT/log

# enable all flags here (will change the setting by commenting in/out lines)
export INSTRUMENT_CPP=1
export INSERT_BUG=1
export INSERT_PARLOOP=1

# name of the test
export TEST_NAME=TestKernel

# compile cpp analysis code
cd $ANALYSIS_ROOT
make clean
make all

# go back to demo root
cd $DEMO_ROOT

# link ~/asp/asp/analysis/work/ to ~/asp/demo/work
if [ -h "$DEMO_ROOT/work" ]; then
    rm $DEMO_ROOT/work
fi
ln -s $ANALYSIS_ROOT/work/ $DEMO_ROOT/work
rm -rf $DEMO_ROOT/work/*
mkdir $DEMO_ROOT/work/$TEST_NAME

# link ~/asp/asp/analysis/log/ to ~/asp/demo/log
if [ -h "$DEMO_ROOT/log" ]; then
    rm $DEMO_ROOT/log
fi
ln -s $ANALYSIS_ROOT/log/ $DEMO_ROOT/log
rm -rf $DEMO_ROOT/log/*

# revert back to original files
cp -f $DEMO_ROOT/demo_backups/emacs $HOME/.emacs
cp -f $DEMO_ROOT/demo_backups/test1_1.py $DEMO_ROOT/tests/test1.py
cp -f $DEMO_ROOT/demo_backups/test2.py $DEMO_ROOT/tests/test2.py
#cp -f $DEMO_ROOT/demo_backups/stencil_convert.py $STENCIL_ROOT/stencil_convert.py
#cp -f $DEMO_ROOT/demo_backups/stencil_kernel.py $STENCIL_ROOT/stencil_kernel.py
#sudo cp -f /usr/lib/pymodules/python2.6/unittest2/runner.py /usr/lib/pymodules/python2.6/unittest2/runner.py.backup
#sudo cp -f $DEMO_ROOT/demo_backups/runner.py /usr/lib/pymodules/python2.6/unittest2/runner.py

# revert back code cache
if [ ! -d "/tmp/asp_cache/" ]; then
    mkdir /tmp/asp_cache/
fi
cp -rf $DEMO_ROOT/asp_cache/* /tmp/asp_cache/

export PYTHON=python
export PYTHONARGS="-W ignore::DeprecationWarning"

# commands to open and run files

opentest1 () {
    emacs -bg $COLOR_TEST $DEMO_ROOT/tests/test1.py
}

oldtest1 () {
    cp -f $DEMO_ROOT/demo_backups/test1_1.py $DEMO_ROOT/tests/test1.py
}

newtest1 () {
    cp -f $DEMO_ROOT/demo_backups/test1_2.py $DEMO_ROOT/tests/test1.py
}

opentest2 () {
    emacs -bg $COLOR_TEST $DEMO_ROOT/tests/test2.py
}

openkernel () {
    emacs -bg $COLOR_ASP $STENCIL_ROOT/stencil_kernel.py
}

openconvert () {
    emacs -bg $COLOR_ASP $STENCIL_ROOT/stencil_convert.py
}

opencpp () {
    emacs -bg $COLOR_CPP $DEMO_ROOT/work/$TEST_NAME/variants.cpp
}

openbuggy () {
    emacs -bg $COLOR_CPP $DEMO_ROOT/work/$TEST_NAME/variants_buggy.cpp
}

opencppbuggy () {
    emacs -bg $COLOR_CPP $DEMO_ROOT/work/$TEST_NAME/variants.cpp $DEMO_ROOT/work/$TEST_NAME/variants_buggy.cpp
}

openuninst () {
    emacs -bg $COLOR_CPP $DEMO_ROOT/work/$TEST_NAME/variants_uninst.cpp
}

openinst () {
    emacs -bg $COLOR_CPP $DEMO_ROOT/work/$TEST_NAME/variants_inst.cpp
}

openreport () {
    emacs -bg $COLOR_REPORT $DEMO_ROOT/work/$TEST_NAME/report.txt
}

opentrace () {
    emacs -bg $COLOR_REPORT /tmp/trace.txt
}

runtest1 () {
    PYTHONPATH=`pwd`:${PYTHONPATH} ${PYTHON} ${PYTHONARGS} $DEMO_ROOT/tests/test1.py
}

runtest2 () {
    PYTHONPATH=`pwd`:${PYTHONPATH} ${PYTHON} ${PYTHONARGS} $DEMO_ROOT/tests/test2.py
}

reset_demo () {
    rm -rf $ASP_ROOT/asp/analysis
    cp -rf $GITHUB_ASP_ROOT/asp/analysis/ $ASP_ROOT/asp/analysis

    rm -rf $ASP_ROOT/asp/codegen
    cp -rf $GITHUB_ASP_ROOT/asp/codegen/ $ASP_ROOT/asp/codegen

    rm -rf $ASP_ROOT/demo
    cp -rf $GITHUB_ASP_ROOT/demo/ $ASP_ROOT/demo

    rm -rf $ASP_ROOT/specializers/stencil
    cp -rf $GITHUB_ASP_ROOT/specializers/stencil/ $ASP_ROOT/specializers/stencil

    source $DEMO_ROOT/reset_demo.sh
}

