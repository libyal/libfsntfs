AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBFSNTFS_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYFSNTFS = \
	test_python_functions.sh
endif

TESTS = \
	test_api_functions.sh \
	test_fsntfsinfo.sh \
	$(TESTS_PYFSNTFS)

check_SCRIPTS = \
	pyfsntfs_test_get_version.py \
	pyfsntfs_test_open_close.py \
	pyfsntfs_test_read.py \
	pyfsntfs_test_seek.py \
	test_api_functions.sh \
	test_fsntfsinfo.sh \
	test_python_functions.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	fsntfs_test_error \
	fsntfs_test_get_version \
	fsntfs_test_open_close \
	fsntfs_test_read \
	fsntfs_test_seek

fsntfs_test_error_SOURCES = \
	fsntfs_test_error.c \
	fsntfs_test_libcstring.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_unused.h

fsntfs_test_error_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCSTRING_LIBADD@

fsntfs_test_get_version_SOURCES = \
	fsntfs_test_get_version.c \
	fsntfs_test_libcstring.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_unused.h

fsntfs_test_get_version_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCSTRING_LIBADD@

fsntfs_test_open_close_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libcstring.h \
	fsntfs_test_libcthreads.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_open_close.c \
	fsntfs_test_unused.h

fsntfs_test_open_close_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

fsntfs_test_read_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libcstring.h \
	fsntfs_test_libcthreads.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_read.c \
	fsntfs_test_unused.h

fsntfs_test_read_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

fsntfs_test_seek_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libcstring.h \
	fsntfs_test_libcthreads.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_seek.c \
	fsntfs_test_unused.h

fsntfs_test_seek_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

