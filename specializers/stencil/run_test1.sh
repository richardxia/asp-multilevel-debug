#!/bin/bash
PYTHONPATH=../../:$PYTHONPATH

if [ -z "${PYTHON}" ]
then
    PYTHON=python
fi
if [ -z "${PYTHONARGS}" ]
then
    PYTHONARGS=
fi

PYTHONPATH=`pwd`:${PYTHONPATH} ${PYTHON} ${PYTHONARGS} tests/test1.py
