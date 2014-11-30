AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFUSN_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
	@LIBFSNTFS_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	fsntfsinfo

fsntfsinfo_SOURCES = \
	fsntfsinfo.c \
	fsntfsoutput.c fsntfsoutput.h \
	fsntfstools_libbfio.h \
	fsntfstools_libcerror.h \
	fsntfstools_libclocale.h \
	fsntfstools_libcnotify.h \
	fsntfstools_libcstring.h \
	fsntfstools_libcsystem.h \
	fsntfstools_libfcache.h \
	fsntfstools_libfdata.h \
	fsntfstools_libfdatetime.h \
	fsntfstools_libfguid.h \
	fsntfstools_libfsntfs.h \
	fsntfstools_libfusn.h \
	fsntfstools_libfwnt.h \
	fsntfstools_libuna.h \
	info_handle.c info_handle.h

fsntfsinfo_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBFUSN_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on fsntfsinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(fsntfsinfo_SOURCES)

