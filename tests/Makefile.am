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
	@LIBFSNTFS_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYFSNTFS = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_fsntfsinfo.sh \
	$(TESTS_PYFSNTFS)

check_SCRIPTS = \
	pyfsntfs_test_read.py \
	pyfsntfs_test_support.py \
	pyfsntfs_test_volume.py \
	test_fsntfsinfo.sh \
	test_library.sh \
	test_python_module.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	fsntfs_test_attribute \
	fsntfs_test_bitmap_values \
	fsntfs_test_cluster_block \
	fsntfs_test_compressed_block \
	fsntfs_test_compressed_block_descriptor \
	fsntfs_test_data_run \
	fsntfs_test_data_stream \
	fsntfs_test_directory \
	fsntfs_test_directory_entry \
	fsntfs_test_error \
	fsntfs_test_file_entry \
	fsntfs_test_file_name_attribute \
	fsntfs_test_file_name_values \
	fsntfs_test_index \
	fsntfs_test_index_entry \
	fsntfs_test_index_node \
	fsntfs_test_index_value \
	fsntfs_test_io_handle \
	fsntfs_test_logged_utility_stream_values \
	fsntfs_test_mft \
	fsntfs_test_mft_entry \
	fsntfs_test_mft_metadata_file \
	fsntfs_test_notify \
	fsntfs_test_object_identifier_values \
	fsntfs_test_reparse_point_attribute \
	fsntfs_test_reparse_point_values \
	fsntfs_test_security_descriptor_index \
	fsntfs_test_security_descriptor_index_value \
	fsntfs_test_security_descriptor_values \
	fsntfs_test_standard_information_values \
	fsntfs_test_support \
	fsntfs_test_txf_data_values \
	fsntfs_test_usn_change_journal \
	fsntfs_test_volume \
	fsntfs_test_volume_header \
	fsntfs_test_volume_information_attribute \
	fsntfs_test_volume_information_values \
	fsntfs_test_volume_name_attribute \
	fsntfs_test_volume_name_values

fsntfs_test_attribute_SOURCES = \
	fsntfs_test_attribute.c \
	fsntfs_test_libcdata.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_attribute_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_bitmap_values_SOURCES = \
	fsntfs_test_bitmap_values.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_bitmap_values_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_cluster_block_SOURCES = \
	fsntfs_test_cluster_block.c \
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_cluster_block_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_compressed_block_SOURCES = \
	fsntfs_test_compressed_block.c \
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_compressed_block_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_compressed_block_descriptor_SOURCES = \
	fsntfs_test_compressed_block_descriptor.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_compressed_block_descriptor_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_data_run_SOURCES = \
	fsntfs_test_data_run.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_data_run_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_data_stream_SOURCES = \
	fsntfs_test_data_stream.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_data_stream_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_directory_SOURCES = \
	fsntfs_test_directory.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_directory_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_directory_entry_SOURCES = \
	fsntfs_test_directory_entry.c \
	fsntfs_test_libcdata.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_directory_entry_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_error_SOURCES = \
	fsntfs_test_error.c \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_unused.h

fsntfs_test_error_LDADD = \
	../libfsntfs/libfsntfs.la

fsntfs_test_file_entry_SOURCES = \
	fsntfs_test_file_entry.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_file_entry_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_file_name_attribute_SOURCES = \
	fsntfs_test_file_name_attribute.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_file_name_attribute_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_file_name_values_SOURCES = \
	fsntfs_test_file_name_values.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_file_name_values_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_index_SOURCES = \
	fsntfs_test_index.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_index_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_index_entry_SOURCES = \
	fsntfs_test_index_entry.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_index_entry_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_index_node_SOURCES = \
	fsntfs_test_index_node.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_index_node_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_index_value_SOURCES = \
	fsntfs_test_index_value.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_index_value_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_io_handle_SOURCES = \
	fsntfs_test_io_handle.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_io_handle_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_logged_utility_stream_values_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_logged_utility_stream_values.c \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_logged_utility_stream_values_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_mft_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_mft.c \
	fsntfs_test_unused.h

fsntfs_test_mft_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_mft_entry_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_mft_entry.c \
	fsntfs_test_unused.h

fsntfs_test_mft_entry_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_mft_metadata_file_SOURCES = \
	fsntfs_test_getopt.c fsntfs_test_getopt.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libclocale.h \
	fsntfs_test_libcnotify.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_libuna.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_mft_metadata_file.c

fsntfs_test_mft_metadata_file_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

fsntfs_test_notify_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_notify.c \
	fsntfs_test_unused.h

fsntfs_test_notify_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_object_identifier_values_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_object_identifier_values.c \
	fsntfs_test_unused.h

fsntfs_test_object_identifier_values_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_reparse_point_attribute_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_reparse_point_attribute.c \
	fsntfs_test_unused.h

fsntfs_test_reparse_point_attribute_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_reparse_point_values_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_reparse_point_values.c \
	fsntfs_test_unused.h

fsntfs_test_reparse_point_values_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_security_descriptor_index_SOURCES = \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_security_descriptor_index.c \
	fsntfs_test_unused.h

fsntfs_test_security_descriptor_index_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_security_descriptor_index_value_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_security_descriptor_index_value.c \
	fsntfs_test_unused.h

fsntfs_test_security_descriptor_index_value_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_security_descriptor_values_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_security_descriptor_values.c \
	fsntfs_test_unused.h

fsntfs_test_security_descriptor_values_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_standard_information_values_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_standard_information_values.c \
	fsntfs_test_unused.h

fsntfs_test_standard_information_values_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_support_SOURCES = \
	fsntfs_test_getopt.c fsntfs_test_getopt.h \
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libclocale.h \
	fsntfs_test_libcnotify.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_libuna.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_support.c \
	fsntfs_test_unused.h

fsntfs_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_txf_data_values_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_txf_data_values.c \
	fsntfs_test_unused.h

fsntfs_test_txf_data_values_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_usn_change_journal_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h \
	fsntfs_test_usn_change_journal.c

fsntfs_test_usn_change_journal_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_volume_SOURCES = \
	fsntfs_test_getopt.c fsntfs_test_getopt.h \
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libclocale.h \
	fsntfs_test_libcnotify.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_libuna.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_volume.c \
	fsntfs_test_unused.h

fsntfs_test_volume_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

fsntfs_test_volume_header_SOURCES = \
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h \
	fsntfs_test_volume_header.c

fsntfs_test_volume_header_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_volume_information_attribute_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h \
	fsntfs_test_volume_information_attribute.c

fsntfs_test_volume_information_attribute_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_volume_information_values_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h \
	fsntfs_test_volume_information_values.c

fsntfs_test_volume_information_values_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_volume_name_attribute_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h \
	fsntfs_test_volume_name_attribute.c

fsntfs_test_volume_name_attribute_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_volume_name_values_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h \
	fsntfs_test_volume_name_values.c

fsntfs_test_volume_name_values_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

