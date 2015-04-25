if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@

pyexec_LTLIBRARIES = pyfsntfs.la

pyfsntfs_la_SOURCES = \
	pyfsntfs.c pyfsntfs.h \
	pyfsntfs_attribute.c pyfsntfs_attribute.h \
	pyfsntfs_attributes.c pyfsntfs_attributes.h \
	pyfsntfs_datetime.c pyfsntfs_datetime.h \
	pyfsntfs_error.c pyfsntfs_error.h \
	pyfsntfs_file_entries.c pyfsntfs_file_entries.h \
	pyfsntfs_file_entry.c pyfsntfs_file_entry.h \
	pyfsntfs_file_name_attribute.c pyfsntfs_file_name_attribute.h \
	pyfsntfs_file_object_io_handle.c pyfsntfs_file_object_io_handle.h \
	pyfsntfs_guid.c pyfsntfs_guid.h \
	pyfsntfs_integer.c pyfsntfs_integer.h \
	pyfsntfs_libbfio.h \
	pyfsntfs_libcerror.h \
	pyfsntfs_libclocale.h \
	pyfsntfs_libcstring.h \
	pyfsntfs_libfguid.h \
	pyfsntfs_libfsntfs.h \
	pyfsntfs_mft_metadata_file.c pyfsntfs_mft_metadata_file.h \
	pyfsntfs_mft_metadata_file_entries.c pyfsntfs_mft_metadata_file_entries.h \
	pyfsntfs_object_identifier_attribute.c pyfsntfs_object_identifier_attribute.h \
	pyfsntfs_python.h \
	pyfsntfs_standard_information_attribute.c pyfsntfs_standard_information_attribute.h \
	pyfsntfs_unused.h \
	pyfsntfs_volume.c pyfsntfs_volume.h \
	pyfsntfs_volume_file_entries.c pyfsntfs_volume_file_entries.h \
	pyfsntfs_volume_information_attribute.c pyfsntfs_volume_information_attribute.h \
	pyfsntfs_volume_name_attribute.c pyfsntfs_volume_name_attribute.h

pyfsntfs_la_LIBADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFGUID_LIBADD@

pyfsntfs_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pyfsntfs_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

EXTRA_DIST = \
	setup.py \
	setup.py.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f setup.py
	/bin/rm -f Makefile

