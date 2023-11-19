AUTOMAKE_OPTIONS = subdir-objects

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
	@LIBFUSN_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBFSNTFS_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYFSNTFS = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_tools.sh \
	test_fsntfsinfo.sh \
	test_fsntfsinfo_bodyfile.sh \
	$(TESTS_PYFSNTFS)

check_SCRIPTS = \
	pyfsntfs_test_attribute.py \
	pyfsntfs_test_file_entry.py \
	pyfsntfs_test_support.py \
	pyfsntfs_test_volume.py \
	test_fsntfsinfo.sh \
	test_fsntfsinfo_bodyfile.sh \
	test_library.sh \
	test_manpage.sh \
	test_python_module.sh \
	test_runner.sh \
	test_tools.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	fsntfs_test_attribute \
	fsntfs_test_attribute_list_entry \
	fsntfs_test_bitmap_values \
	fsntfs_test_buffer_data_handle \
	fsntfs_test_cluster_block \
	fsntfs_test_cluster_block_data \
	fsntfs_test_cluster_block_stream \
	fsntfs_test_cluster_block_vector \
	fsntfs_test_compressed_block \
	fsntfs_test_compressed_block_data_handle \
	fsntfs_test_compressed_block_vector \
	fsntfs_test_compressed_data_handle \
	fsntfs_test_compression \
	fsntfs_test_compression_unit_data_handle \
	fsntfs_test_compression_unit_descriptor \
	fsntfs_test_data_run \
	fsntfs_test_data_stream \
	fsntfs_test_directory_entries_tree \
	fsntfs_test_directory_entry \
	fsntfs_test_error \
	fsntfs_test_extent \
	fsntfs_test_file_entry \
	fsntfs_test_file_name_attribute \
	fsntfs_test_file_name_values \
	fsntfs_test_file_system \
	fsntfs_test_fixup_values \
	fsntfs_test_index \
	fsntfs_test_index_entry \
	fsntfs_test_index_entry_header \
	fsntfs_test_index_entry_vector \
	fsntfs_test_index_node \
	fsntfs_test_index_node_header \
	fsntfs_test_index_root_header \
	fsntfs_test_index_value \
	fsntfs_test_io_handle \
	fsntfs_test_logged_utility_stream_values \
	fsntfs_test_mft \
	fsntfs_test_mft_attribute \
	fsntfs_test_mft_attribute_list \
	fsntfs_test_mft_attribute_list_entry \
	fsntfs_test_mft_entry \
	fsntfs_test_mft_entry_header \
	fsntfs_test_mft_metadata_file \
	fsntfs_test_name \
	fsntfs_test_notify \
	fsntfs_test_object_identifier_values \
	fsntfs_test_path_hint \
	fsntfs_test_profiler \
	fsntfs_test_reparse_point_attribute \
	fsntfs_test_reparse_point_values \
	fsntfs_test_sds_index_value \
	fsntfs_test_security_descriptor_index \
	fsntfs_test_security_descriptor_index_value \
	fsntfs_test_security_descriptor_values \
	fsntfs_test_standard_information_values \
	fsntfs_test_support \
	fsntfs_test_tools_bodyfile \
	fsntfs_test_tools_digest_hash \
	fsntfs_test_tools_info_handle \
	fsntfs_test_tools_mount_path_string \
	fsntfs_test_tools_output \
	fsntfs_test_tools_path_string \
	fsntfs_test_tools_signal \
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
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_rwlock.c fsntfs_test_rwlock.h \
	fsntfs_test_unused.h

fsntfs_test_attribute_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

fsntfs_test_attribute_list_entry_SOURCES = \
	fsntfs_test_attribute_list_entry.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_attribute_list_entry_LDADD = \
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

fsntfs_test_buffer_data_handle_SOURCES = \
	fsntfs_test_buffer_data_handle.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_buffer_data_handle_LDADD = \
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

fsntfs_test_cluster_block_data_SOURCES = \
	fsntfs_test_cluster_block_data.c \
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfdata.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_cluster_block_data_LDADD = \
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

fsntfs_test_cluster_block_stream_SOURCES = \
	fsntfs_test_cluster_block_stream.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfdata.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_cluster_block_stream_LDADD = \
	@LIBFDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_cluster_block_vector_SOURCES = \
	fsntfs_test_cluster_block_vector.c \
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfcache.h \
	fsntfs_test_libfdata.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_cluster_block_vector_LDADD = \
	@LIBFDATA_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
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
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_compressed_block_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_compressed_block_data_handle_SOURCES = \
	fsntfs_test_compressed_block_data_handle.c \
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_compressed_block_data_handle_LDADD = \
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

fsntfs_test_compressed_block_vector_SOURCES = \
	fsntfs_test_compressed_block_vector.c \
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfcache.h \
	fsntfs_test_libfdata.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_compressed_block_vector_LDADD = \
	@LIBFDATA_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
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

fsntfs_test_compressed_data_handle_SOURCES = \
	fsntfs_test_compressed_data_handle.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfdata.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_compressed_data_handle_LDADD = \
	@LIBFDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_compression_SOURCES = \
	fsntfs_test_compression.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_unused.h

fsntfs_test_compression_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_compression_unit_data_handle_SOURCES = \
	fsntfs_test_compression_unit_data_handle.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_compression_unit_data_handle_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_compression_unit_descriptor_SOURCES = \
	fsntfs_test_compression_unit_descriptor.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_compression_unit_descriptor_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_data_run_SOURCES = \
	fsntfs_test_data_run.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfdata.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_data_run_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_data_stream_SOURCES = \
	fsntfs_test_data_stream.c \
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_rwlock.c fsntfs_test_rwlock.h \
	fsntfs_test_unused.h

fsntfs_test_data_stream_LDADD = \
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
	@PTHREAD_LIBADD@

fsntfs_test_directory_entries_tree_SOURCES = \
	fsntfs_test_directory_entries_tree.c \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_directory_entries_tree_LDADD = \
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

fsntfs_test_directory_entry_SOURCES = \
	fsntfs_test_directory_entry.c \
	fsntfs_test_libcdata.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_directory_entry_LDADD = \
	@LIBCDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_error_SOURCES = \
	fsntfs_test_error.c \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_unused.h

fsntfs_test_error_LDADD = \
	../libfsntfs/libfsntfs.la

fsntfs_test_extent_SOURCES = \
	fsntfs_test_extent.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_extent_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_file_entry_SOURCES = \
	fsntfs_test_file_entry.c \
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_rwlock.c fsntfs_test_rwlock.h \
	fsntfs_test_unused.h

fsntfs_test_file_entry_LDADD = \
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
	@PTHREAD_LIBADD@

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

fsntfs_test_file_system_SOURCES = \
	fsntfs_test_file_system.c \
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_rwlock.c fsntfs_test_rwlock.h \
	fsntfs_test_unused.h

fsntfs_test_file_system_LDADD = \
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
	@PTHREAD_LIBADD@

fsntfs_test_fixup_values_SOURCES = \
	fsntfs_test_fixup_values.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_unused.h

fsntfs_test_fixup_values_LDADD = \
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
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_index_entry.c \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_index_entry_LDADD = \
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

fsntfs_test_index_entry_header_SOURCES = \
	fsntfs_test_index_entry_header.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_index_entry_header_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_index_entry_vector_SOURCES = \
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_index_entry_vector.c \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfcache.h \
	fsntfs_test_libfdata.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_index_entry_vector_LDADD = \
	@LIBFDATA_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
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

fsntfs_test_index_node_header_SOURCES = \
	fsntfs_test_index_node_header.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_index_node_header_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_index_root_header_SOURCES = \
	fsntfs_test_index_root_header.c \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_unused.h

fsntfs_test_index_root_header_LDADD = \
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

fsntfs_test_mft_attribute_SOURCES = \
	fsntfs_test_libcdata.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_mft_attribute.c \
	fsntfs_test_unused.h

fsntfs_test_mft_attribute_LDADD = \
	@LIBCDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_mft_attribute_list_SOURCES = \
	fsntfs_test_libcdata.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_mft_attribute_list.c \
	fsntfs_test_unused.h

fsntfs_test_mft_attribute_list_LDADD = \
	@LIBCDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_mft_attribute_list_entry_SOURCES = \
	fsntfs_test_libcdata.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_mft_attribute_list_entry.c \
	fsntfs_test_unused.h

fsntfs_test_mft_attribute_list_entry_LDADD = \
	@LIBCDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_mft_entry_SOURCES = \
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_mft_entry.c \
	fsntfs_test_unused.h

fsntfs_test_mft_entry_LDADD = \
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

fsntfs_test_mft_entry_header_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_mft_entry_header.c \
	fsntfs_test_unused.h

fsntfs_test_mft_entry_header_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_mft_metadata_file_SOURCES = \
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_getopt.c fsntfs_test_getopt.h \
	fsntfs_test_libbfio.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libclocale.h \
	fsntfs_test_libcnotify.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_libuna.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_mft_metadata_file.c \
	fsntfs_test_rwlock.c fsntfs_test_rwlock.h

fsntfs_test_mft_metadata_file_LDADD = \
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
	@PTHREAD_LIBADD@

fsntfs_test_name_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_libuna.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_name.c \
	fsntfs_test_unused.h

fsntfs_test_name_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

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

fsntfs_test_path_hint_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_path_hint.c \
	fsntfs_test_unused.h

fsntfs_test_path_hint_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_profiler_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_profiler.c \
	fsntfs_test_unused.h

fsntfs_test_profiler_LDADD = \
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

fsntfs_test_sds_index_value_SOURCES = \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_sds_index_value.c \
	fsntfs_test_unused.h

fsntfs_test_sds_index_value_LDADD = \
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
	fsntfs_test_functions.c fsntfs_test_functions.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libfdata.h \
	fsntfs_test_libfsntfs.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_security_descriptor_values.c \
	fsntfs_test_unused.h

fsntfs_test_security_descriptor_values_LDADD = \
	@LIBFDATA_LIBADD@ \
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

fsntfs_test_tools_bodyfile_SOURCES = \
	../fsntfstools/bodyfile.c ../fsntfstools/bodyfile.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libcpath.h \
	fsntfs_test_macros.h \
	fsntfs_test_tools_bodyfile.c \
	fsntfs_test_unused.h

fsntfs_test_tools_bodyfile_LDADD = \
	@LIBUNA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_tools_digest_hash_SOURCES = \
	../fsntfstools/digest_hash.c ../fsntfstools/digest_hash.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_macros.h \
	fsntfs_test_tools_digest_hash.c \
	fsntfs_test_unused.h

fsntfs_test_tools_digest_hash_LDADD = \
	@LIBCERROR_LIBADD@

fsntfs_test_tools_info_handle_SOURCES = \
	../fsntfstools/bodyfile.c ../fsntfstools/bodyfile.h \
	../fsntfstools/digest_hash.c ../fsntfstools/digest_hash.h \
	../fsntfstools/info_handle.c ../fsntfstools/info_handle.h \
	../fsntfstools/path_string.c ../fsntfstools/path_string.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_macros.h \
	fsntfs_test_memory.c fsntfs_test_memory.h \
	fsntfs_test_tools_info_handle.c \
	fsntfs_test_unused.h

fsntfs_test_tools_info_handle_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFUSN_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_tools_mount_path_string_SOURCES = \
	../fsntfstools/mount_path_string.c ../fsntfstools/mount_path_string.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_macros.h \
	fsntfs_test_tools_mount_path_string.c \
	fsntfs_test_unused.h

fsntfs_test_tools_mount_path_string_LDADD = \
	@LIBUNA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_tools_output_SOURCES = \
	../fsntfstools/fsntfstools_output.c ../fsntfstools/fsntfstools_output.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_macros.h \
	fsntfs_test_tools_output.c \
	fsntfs_test_unused.h

fsntfs_test_tools_output_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_tools_path_string_SOURCES = \
	../fsntfstools/path_string.c ../fsntfstools/path_string.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_libcpath.h \
	fsntfs_test_macros.h \
	fsntfs_test_tools_path_string.c \
	fsntfs_test_unused.h

fsntfs_test_tools_path_string_LDADD = \
	@LIBUNA_LIBADD@ \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@

fsntfs_test_tools_signal_SOURCES = \
	../fsntfstools/fsntfstools_signal.c ../fsntfstools/fsntfstools_signal.h \
	fsntfs_test_libcerror.h \
	fsntfs_test_macros.h \
	fsntfs_test_tools_signal.c \
	fsntfs_test_unused.h

fsntfs_test_tools_signal_LDADD = \
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
	fsntfs_test_rwlock.c fsntfs_test_rwlock.h \
	fsntfs_test_unused.h \
	fsntfs_test_usn_change_journal.c

fsntfs_test_usn_change_journal_LDADD = \
	../libfsntfs/libfsntfs.la \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

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
	fsntfs_test_rwlock.c fsntfs_test_rwlock.h \
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
	-rm -f Makefile

