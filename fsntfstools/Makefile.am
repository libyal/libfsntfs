AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
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
	@LIBHMAC_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBFSNTFS_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	fsntfsinfo \
	fsntfsmount

fsntfsinfo_SOURCES = \
	bodyfile.c bodyfile.h \
	digest_hash.c digest_hash.h \
	fsntfsinfo.c \
	fsntfstools_getopt.c fsntfstools_getopt.h \
	fsntfstools_i18n.h \
	fsntfstools_libbfio.h \
	fsntfstools_libcerror.h \
	fsntfstools_libclocale.h \
	fsntfstools_libcnotify.h \
	fsntfstools_libfcache.h \
	fsntfstools_libfdata.h \
	fsntfstools_libfdatetime.h \
	fsntfstools_libfguid.h \
	fsntfstools_libfsntfs.h \
	fsntfstools_libfusn.h \
	fsntfstools_libfwnt.h \
	fsntfstools_libhmac.h \
	fsntfstools_libuna.h \
	fsntfstools_output.c fsntfstools_output.h \
	fsntfstools_signal.c fsntfstools_signal.h \
	fsntfstools_unused.h \
	info_handle.c info_handle.h \
	path_string.c path_string.h

fsntfsinfo_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFUSN_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

fsntfsmount_SOURCES = \
	fsntfsmount.c \
	fsntfstools_getopt.c fsntfstools_getopt.h \
	fsntfstools_i18n.h \
	fsntfstools_libbfio.h \
	fsntfstools_libcerror.h \
	fsntfstools_libclocale.h \
	fsntfstools_libcnotify.h \
	fsntfstools_libcpath.h \
	fsntfstools_libfsntfs.h \
	fsntfstools_libuna.h \
	fsntfstools_output.c fsntfstools_output.h \
	fsntfstools_signal.c fsntfstools_signal.h \
	fsntfstools_unused.h \
	mount_dokan.c mount_dokan.h \
	mount_file_entry.c mount_file_entry.h \
	mount_file_system.c mount_file_system.h \
	mount_fuse.c mount_fuse.h \
	mount_handle.c mount_handle.h \
	mount_path_string.c mount_path_string.h

fsntfsmount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f Makefile

splint-local:
	@echo "Running splint on fsntfsinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(fsntfsinfo_SOURCES)

