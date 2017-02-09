/*
 * File entry functions
 *
 * Copyright (C) 2010-2017, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBFSNTFS_INTERNAL_FILE_ENTRY_H )
#define _LIBFSNTFS_INTERNAL_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsntfs_directory_entry.h"
#include "libfsntfs_extern.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_security_descriptor_index.h"
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

	/* The MFT
	 */
	libfsntfs_mft_t *mft;

	/* The MFT entry
	 */
	libfsntfs_mft_entry_t *mft_entry;

	/* The directory entry
	 */
	libfsntfs_directory_entry_t *directory_entry;

	/* The security descriptor index
	 */
	libfsntfs_security_descriptor_index_t *security_descriptor_index;

	/* The security descriptor values
	 */
	libfsntfs_security_descriptor_values_t *security_descriptor_values;

	/* The data size
	 */
	size64_t data_size;

	/* The default (nameless) $DATA attribute
	 */
	libfsntfs_attribute_t *data_attribute;

	/* The default (nameless) $DATA attribute cluster block stream
	 */
	libfdata_stream_t *data_cluster_block_stream;

	/* The directory entries tree
	 */
	libcdata_btree_t *directory_entries_tree;

	/* The flags
	 */
	uint8_t flags;
};

int libfsntfs_file_entry_initialize(
     libfsntfs_file_entry_t **file_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_t *mft,
     libfsntfs_security_descriptor_index_t *security_descriptor_index,
     libfsntfs_mft_entry_t *mft_entry,
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
     uint64_t *file_reference,
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
     uint64_t *creation_time,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_modification_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *modification_time,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_access_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *access_time,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_entry_modification_time(
     libfsntfs_file_entry_t *file_entry,
     uint64_t *entry_modification_time,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_file_attribute_flags(
     libfsntfs_file_entry_t *file_entry,
     uint32_t *file_attribute_flags,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf8_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf8_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf8_name(
     libfsntfs_file_entry_t *file_entry,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf16_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_name(
     libfsntfs_file_entry_t *file_entry,
     uint16_t *utf16_name,
     size_t utf16_name_size,
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
     size_t *utf8_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf8_name_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_name_size_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     size_t *utf16_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_name_by_attribute_index(
     libfsntfs_file_entry_t *file_entry,
     int attribute_index,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf8_reparse_point_substitute_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf8_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf8_reparse_point_substitute_name(
     libfsntfs_file_entry_t *file_entry,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_reparse_point_substitute_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf16_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_reparse_point_substitute_name(
     libfsntfs_file_entry_t *file_entry,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf8_reparse_point_print_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf8_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf8_reparse_point_print_name(
     libfsntfs_file_entry_t *file_entry,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_reparse_point_print_name_size(
     libfsntfs_file_entry_t *file_entry,
     size_t *utf16_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_file_entry_get_utf16_reparse_point_print_name(
     libfsntfs_file_entry_t *file_entry,
     uint16_t *utf16_name,
     size_t utf16_name_size,
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

#endif /* !defined( _LIBFSNTFS_INTERNAL_FILE_ENTRY_H ) */

