if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFSNTFS_DLL_IMPORT@

pyexec_LTLIBRARIES = pyfsntfs.la

pyfsntfs_la_SOURCES = \
	pyfsntfs.c pyfsntfs.h \
	pyfsntfs_attribute.c pyfsntfs_attribute.h \
	pyfsntfs_attribute_types.c pyfsntfs_attribute_types.h \
	pyfsntfs_attributes.c pyfsntfs_attributes.h \
	pyfsntfs_data_stream.c pyfsntfs_data_stream.h \
	pyfsntfs_data_streams.c pyfsntfs_data_streams.h \
	pyfsntfs_datetime.c pyfsntfs_datetime.h \
	pyfsntfs_error.c pyfsntfs_error.h \
	pyfsntfs_file_attribute_flags.c pyfsntfs_file_attribute_flags.h \
	pyfsntfs_file_entries.c pyfsntfs_file_entries.h \
	pyfsntfs_file_entry.c pyfsntfs_file_entry.h \
	pyfsntfs_file_name_attribute.c pyfsntfs_file_name_attribute.h \
	pyfsntfs_file_object_io_handle.c pyfsntfs_file_object_io_handle.h \
	pyfsntfs_guid.c pyfsntfs_guid.h \
	pyfsntfs_integer.c pyfsntfs_integer.h \
	pyfsntfs_libbfio.h \
	pyfsntfs_libcerror.h \
	pyfsntfs_libclocale.h \
	pyfsntfs_libfguid.h \
	pyfsntfs_libfsntfs.h \
	pyfsntfs_libuna.h \
	pyfsntfs_mft_metadata_file.c pyfsntfs_mft_metadata_file.h \
	pyfsntfs_mft_metadata_file_entries.c pyfsntfs_mft_metadata_file_entries.h \
	pyfsntfs_object_identifier_attribute.c pyfsntfs_object_identifier_attribute.h \
	pyfsntfs_python.h \
	pyfsntfs_reparse_point_attribute.c pyfsntfs_reparse_point_attribute.h \
	pyfsntfs_security_descriptor_attribute.c pyfsntfs_security_descriptor_attribute.h \
	pyfsntfs_standard_information_attribute.c pyfsntfs_standard_information_attribute.h \
	pyfsntfs_string.c pyfsntfs_string.h \
	pyfsntfs_usn_change_journal.c pyfsntfs_usn_change_journal.h \
	pyfsntfs_unused.h \
	pyfsntfs_volume.c pyfsntfs_volume.h \
	pyfsntfs_volume_file_entries.c pyfsntfs_volume_file_entries.h \
	pyfsntfs_volume_information_attribute.c pyfsntfs_volume_information_attribute.h \
	pyfsntfs_volume_name_attribute.c pyfsntfs_volume_name_attribute.h

pyfsntfs_la_LIBADD = \
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

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f Makefile

