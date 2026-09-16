#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libfsntfs/.libs/libfsntfs.1.dylib" ] && [ -f ./pyfsntfs/.libs/pyfsntfs.so ]
then
    install_name_tool -change /usr/local/lib/libfsntfs.1.dylib "${PWD}/libfsntfs/.libs/libfsntfs.1.dylib" ./pyfsntfs/.libs/pyfsntfs.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

