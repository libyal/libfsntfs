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
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBFSNTFS_DLL_EXPORT@

lib_LTLIBRARIES = libfsntfs.la

libfsntfs_la_SOURCES = \
	fsntfs_attribute_list.h \
	fsntfs_file_name.h \
	fsntfs_index.h \
	fsntfs_logged_utility_stream.h \
	fsntfs_mft_attribute.h \
	fsntfs_mft_entry.h \
	fsntfs_object_identifier.h \
	fsntfs_reparse_point.h \
	fsntfs_secure.h \
	fsntfs_standard_information.h \
	fsntfs_txf_data.h \
	fsntfs_volume_header.h \
	fsntfs_volume_information.h \
	libfsntfs.c \
	libfsntfs_attribute.c libfsntfs_attribute.h \
	libfsntfs_attribute_list.c libfsntfs_attribute_list.h \
	libfsntfs_bitmap_values.c libfsntfs_bitmap_values.h \
	libfsntfs_cluster_block.c libfsntfs_cluster_block.h \
	libfsntfs_cluster_block_stream.c libfsntfs_cluster_block_stream.h \
	libfsntfs_cluster_block_vector.c libfsntfs_cluster_block_vector.h \
	libfsntfs_compressed_block.c libfsntfs_compressed_block.h \
	libfsntfs_compressed_block_descriptor.c libfsntfs_compressed_block_descriptor.h \
	libfsntfs_data_run.c libfsntfs_data_run.h \
	libfsntfs_data_stream.c libfsntfs_data_stream.h \
	libfsntfs_debug.c libfsntfs_debug.h \
	libfsntfs_definitions.h \
	libfsntfs_directory.c libfsntfs_directory.h \
	libfsntfs_directory_entries_tree.c libfsntfs_directory_entries_tree.h \
	libfsntfs_directory_entry.c libfsntfs_directory_entry.h \
	libfsntfs_error.c libfsntfs_error.h \
	libfsntfs_extern.h \
	libfsntfs_file_entry.c libfsntfs_file_entry.h \
	libfsntfs_file_name_attribute.c libfsntfs_file_name_attribute.h \
	libfsntfs_file_name_values.c libfsntfs_file_name_values.h \
	libfsntfs_index.c libfsntfs_index.h \
	libfsntfs_index_entry.c libfsntfs_index_entry.h \
	libfsntfs_index_entry_vector.c libfsntfs_index_entry_vector.h \
	libfsntfs_index_node.c libfsntfs_index_node.h \
	libfsntfs_index_value.c libfsntfs_index_value.h \
	libfsntfs_io_handle.c libfsntfs_io_handle.h \
	libfsntfs_libbfio.h \
	libfsntfs_libcdata.h \
	libfsntfs_libcerror.h \
	libfsntfs_libclocale.h \
	libfsntfs_libcnotify.h \
	libfsntfs_libcthreads.h \
	libfsntfs_libfcache.h \
	libfsntfs_libfdata.h \
	libfsntfs_libfdatetime.h \
	libfsntfs_libfguid.h \
	libfsntfs_libfwnt.h \
	libfsntfs_libuna.h \
	libfsntfs_logged_utility_stream_values.c libfsntfs_logged_utility_stream_values.h \
	libfsntfs_mft.c libfsntfs_mft.h \
	libfsntfs_mft_entry.c libfsntfs_mft_entry.h \
	libfsntfs_mft_metadata_file.c libfsntfs_mft_metadata_file.h \
	libfsntfs_notify.c libfsntfs_notify.h \
	libfsntfs_object_identifier_attribute.c libfsntfs_object_identifier_attribute.h \
	libfsntfs_object_identifier_values.c libfsntfs_object_identifier_values.h \
	libfsntfs_reparse_point_attribute.c libfsntfs_reparse_point_attribute.h \
	libfsntfs_reparse_point_values.c libfsntfs_reparse_point_values.h \
	libfsntfs_security_descriptor_attribute.c libfsntfs_security_descriptor_attribute.h \
	libfsntfs_security_descriptor_index.c libfsntfs_security_descriptor_index.h \
	libfsntfs_security_descriptor_index_value.c libfsntfs_security_descriptor_index_value.h \
	libfsntfs_security_descriptor_values.c libfsntfs_security_descriptor_values.h \
	libfsntfs_standard_information_attribute.c libfsntfs_standard_information_attribute.h \
	libfsntfs_standard_information_values.c libfsntfs_standard_information_values.h \
	libfsntfs_support.c libfsntfs_support.h \
	libfsntfs_types.h \
	libfsntfs_txf_data_values.c libfsntfs_txf_data_values.h \
	libfsntfs_unused.h \
	libfsntfs_usn_change_journal.c libfsntfs_usn_change_journal.h \
	libfsntfs_volume.c libfsntfs_volume.h \
	libfsntfs_volume_header.c libfsntfs_volume_header.h \
	libfsntfs_volume_information_attribute.c libfsntfs_volume_information_attribute.h \
	libfsntfs_volume_information_values.c libfsntfs_volume_information_values.h \
	libfsntfs_volume_name_attribute.c libfsntfs_volume_name_attribute.h \
	libfsntfs_volume_name_values.c libfsntfs_volume_name_values.h

libfsntfs_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@PTHREAD_LIBADD@

libfsntfs_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libfsntfs_definitions.h.in \
	libfsntfs.rc \
	libfsntfs.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libfsntfs_definitions.h
	/bin/rm -f libfsntfs.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libfsntfs ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libfsntfs_la_SOURCES)

