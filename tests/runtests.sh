#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libfsntfs/.libs/libfsntfs.1.dylib && test -f ./pyfsntfs/.libs/pyfsntfs.so
then
	install_name_tool -change /usr/local/lib/libfsntfs.1.dylib ${PWD}/libfsntfs/.libs/libfsntfs.1.dylib ./pyfsntfs/.libs/pyfsntfs.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

