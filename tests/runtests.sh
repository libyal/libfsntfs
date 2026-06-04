#!/bin/sh
# Script to run tests
#
# Version: 20260602

if test -f ${PWD}/libfsntfs/.libs/libfsntfs.1.dylib && test -f ./pyfsntfs/.libs/pyfsntfs.so
then
	install_name_tool -change /usr/local/lib/libfsntfs.1.dylib ${PWD}/libfsntfs/.libs/libfsntfs.1.dylib ./pyfsntfs/.libs/pyfsntfs.so
fi

make check $@
RESULT=$?

if test ${RESULT} -ne 0 && test -f tests/test-suite.log
then
	cat tests/test-suite.log
fi
exit ${RESULT}

