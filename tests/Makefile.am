AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBFSNTFS_DLL_IMPORT@

TESTS = \
	test_fsntfsinfo.sh

check_SCRIPTS = \
	test_fsntfsinfo.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS =

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

