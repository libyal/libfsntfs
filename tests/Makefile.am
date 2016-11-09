AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
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
	fsntfs_test_open_close \
	fsntfs_test_read \
	fsntfs_test_seek \
	fsntfs_test_support

fsntfs_test_error_SOURCES = \
	fsntfs_test_error.c \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_unused.h

fsntfs_test_error_LDADD = \
	../libfsntfs/libfsntfs.la

fsntfs_test_open_close_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libcthreads.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_open_close.c \
	fsntfs_test_unused.h

fsntfs_test_open_close_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@
	@PTHREAD_LIBADD@

fsntfs_test_read_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libcthreads.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_read.c \
	fsntfs_test_unused.h

fsntfs_test_read_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@
	@PTHREAD_LIBADD@

fsntfs_test_seek_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libcthreads.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_seek.c \
	fsntfs_test_unused.h

fsntfs_test_seek_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@
	@PTHREAD_LIBADD@

fsntfs_test_support_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_support.c \
	fsntfs_test_unused.h

fsntfs_test_support_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

