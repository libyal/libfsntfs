/*
 * File entry functions
 *
 * Copyright (C) 2010-2023, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBFSNTFS_FILE_ENTRY_H )
#define _LIBFSNTFS_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsntfs_directory_entries_tree.h"
#include "libfsntfs_directory_entry.h"
#include "libfsntfs_extern.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_file_system.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcthreads.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_path_hint.h"
#include "libfsntfs_security_descriptor_values.h"
#include "libfsntfs_types.h"
#include "libfsntfs_volume.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_internal_file_entry libfsntfs_internal_file_entry_t;

struct libfsntfs_internal_file_entry
{
	/* The IO handle
	 */
	libfsntfs_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The file system
	 */
	libfsntfs_file_system_t *file_system;

	/* The MFT entry
	 */
	libfsntfs_mft_entry_t *mft_entry;

	/* The directory entry
	 */
	libfsntfs_directory_entry_t *directory_entry;

	/* The directory entries tree
	 */
	libfsntfs_directory_entries_tree_t *directory_entries_tree;

	/* The security descriptor values
	 */
	libfsntfs_security_descriptor_values_t *security_descriptor_values;

	/* The default (nameless) $DATA attribute
	 */
	libfsntfs_mft_attribute_t *data_attribute;

	/* The data extents array
	 */
	libcdata_array_t *extents_array;

	/* The attributes array
	 */
	libcdata_array_t *attributes_array;

	/* The reparse point ($REPARSE_POINT) attribute
	 */
	libfsntfs_attribute_t *reparse_point_attribute;

	/* The security descriptor ($SECURITY_DESCRIPTOR) attribute
	 */
	libfsntfs_attribute_t *security_descriptor_attribute;

	/* The standard information ($STANDARD_INFORMATION) attribute
	 */
	libfsntfs_attribute_t *standard_information_attribute;

	/* The default (nameless) $DATA attribute cluster block stream
	 */
	libfdata_stream_t *data_cluster_block_stream;

	/* The flags
	 */
	uint8_t flags;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfsntfs_file_entry_initialize(
     libfsntfs_file_entry_t **file_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_file_system_t *file_system,
     uint64_t mft_entry_index,
     libfsntfs_directory_entry_t *directory_entry,
     uint8_t flags,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_free(
     libfsntfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_is_empty(
     libfsntfs_file_entry_t *file_entry,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_is_allocated(
     libfsntfs_file_entry_t *file_entry,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_is_corrupted(
     libfsntfs_file_entry_t *file_entry,
     libcerror_error_t **error );

int libfsntfs_internal_file_entry_get_attribute_by_index(
     libfsntfs_internal_file_entry_t *internal_file_entry,
     libfsntfs_mft_entry_t *mft_entry,
     int attribute_index,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_internal_file_entry_get_reparse_point_attribute(
     libfsntfs_internal_file_entry_t *internal_file_entry,
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_internal_file_entry_get_security_descriptor_attribute(
     libfsntfs_internal_file_entry_t *internal_file_entry,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_internal_file_entry_get_standard_information_attribute(
     libfsntfs_internal_file_entry_t *internal_file_entry,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_file_reference(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *file_reference,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_base_record_file_reference(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *file_reference,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_parent_file_reference(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *parent_file_reference,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_parent_file_reference_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     uint64_t *file_reference,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_journal_sequence_number(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *journal_sequence_number,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_creation_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *filetime,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_modification_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *filetime,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_access_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *filetime,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_entry_modification_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *filetime,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_file_attribute_flags(
     libfsntfs_file_entry_t *file_entry,
     uint32_t *file_attribute_flags,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf8_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf8_name(
     libfsntfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_name(
     libfsntfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_name_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int *attribute_index,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf8_name_size_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf8_name_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_name_size_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_name_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfsntfs_internal_file_entry_get_path_hint(
     libfsntfs_internal_file_entry_t *internal_file_entry,
     int attribute_index,
     libfsntfs_path_hint_t **path_hint,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf8_path_hint_size(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf8_path_hint(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_path_hint_size(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_path_hint(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf8_symbolic_link_target_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf8_symbolic_link_target(
     libfsntfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_symbolic_link_target_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_symbolic_link_target(
     libfsntfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_security_descriptor_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *data_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_security_descriptor(
     libfsntfs_file_entry_t *file_entry,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_number_of_attributes(
     libfsntfs_file_entry_t *file_entry,
     int *number_of_attributes,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_attribute_by_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_has_directory_entries_index(
     libfsntfs_file_entry_t *file_entry,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_has_default_data_stream(
     libfsntfs_file_entry_t *file_entry,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_is_symbolic_link(
     libfsntfs_file_entry_t *file_entry,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_number_of_alternate_data_streams(
     libfsntfs_file_entry_t *file_entry,
     int *number_of_alternate_data_streams,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_alternate_data_stream_by_index(
     libfsntfs_file_entry_t *file_entry,
     int alternate_data_stream_index,
     libfsntfs_data_stream_t **alternate_data_stream,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_has_alternate_data_stream_by_utf8_name(
     libfsntfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_has_alternate_data_stream_by_utf16_name(
     libfsntfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_alternate_data_stream_by_utf8_name(
     libfsntfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_data_stream_t **alternate_data_stream,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_alternate_data_stream_by_utf16_name(
     libfsntfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_data_stream_t **alternate_data_stream,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_number_of_sub_file_entries(
     libfsntfs_file_entry_t *file_entry,
     int *number_of_sub_entries,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_sub_file_entry_by_index(
     libfsntfs_file_entry_t *file_entry,
     int sub_file_entry_index,
     libfsntfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_sub_file_entry_by_utf8_name(
     libfsntfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_sub_file_entry_by_utf16_name(
     libfsntfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
ssize_t libfsntfs_file_entry_read_buffer(
         libfsntfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBFSNTFS_EXTERN \
ssize_t libfsntfs_file_entry_read_buffer_at_offset(
         libfsntfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBFSNTFS_EXTERN \
off64_t libfsntfs_file_entry_seek_offset(
         libfsntfs_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_offset(
     libfsntfs_file_entry_t *file_entry,
     off64_t *offset,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_size(
     libfsntfs_file_entry_t *file_entry,
     size64_t *size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_number_of_extents(
     libfsntfs_file_entry_t *file_entry,
     int *number_of_extents,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_extent_by_index(
     libfsntfs_file_entry_t *file_entry,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_FILE_ENTRY_H ) */

