AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBFSNTFS_DLL_IMPORT@

TESTS = \
	test_open_close.sh \
	test_read.sh \
	test_seek.sh \
	test_fsntfsinfo.sh

check_SCRIPTS = \
	test_fsntfsinfo.sh \
	test_open_close.sh \
	test_read.sh \
	test_runner.sh \
	test_seek.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	fsntfs_test_open_close \
	fsntfs_test_read \
	fsntfs_test_seek

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

