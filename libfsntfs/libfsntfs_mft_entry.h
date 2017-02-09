/*
 * Master File Table (MFT) entry functions
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

#if !defined( _LIBFSNTFS_MFT_ENTRY_H )
#define _LIBFSNTFS_MFT_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsntfs_attribute.h"
#include "libfsntfs_directory_entry.h"
#include "libfsntfs_index.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_mft_entry libfsntfs_mft_entry_t;

struct libfsntfs_mft_entry
{
	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;

	/* The flags
	 */
	uint16_t flags;

	/* The index
	 */
	uint32_t index;

	/* The sequence
	 */
	uint16_t sequence;

	/* The journal sequence number
	 */
	uint64_t journal_sequence_number;

	/* The reference count
	 */
	uint16_t reference_count;

	/* The base record file reference
	 */
	uint64_t base_record_file_reference;

	/* The used entry size 
	 */
	uint16_t used_entry_size;

	/* The attributes offset
	 */
	uint16_t attributes_offset;

	/* The attributes array
	 */
	libcdata_array_t *attributes_array;

	/* The default (nameless) $DATA attribute
	 */
	libfsntfs_attribute_t *data_attribute;

	/* The alternate data attributes array
	 */
	libcdata_array_t *alternate_data_attributes_array;

	/* The object identifier attribute
	 */
	libfsntfs_attribute_t *object_identifier_attribute;

	/* The reparse point attribute
	 */
	libfsntfs_attribute_t *reparse_point_attribute;

	/* The standard information attribute
	 */
	libfsntfs_attribute_t *standard_information_attribute;

	/* The security descriptor attribute
	 */
	libfsntfs_attribute_t *security_descriptor_attribute;

	/* The volume information attribute
	 */
	libfsntfs_attribute_t *volume_information_attribute;

	/* The volume name attribute
	 */
	libfsntfs_attribute_t *volume_name_attibute;

	/* The index array
	 */
	libcdata_array_t *index_array;

	/* The $I30 (directory entry) index
	 */
	libfsntfs_index_t *i30_index;

	/* The $SII (security ID) index
	 */
	libfsntfs_index_t *sii_index;

	/* Value to indicate the MFT entry is empty
	 */
	uint8_t is_empty;

	/* Value to indicate the MFT entry is corrupted
	 */
	uint8_t is_corrupted;
};

int libfsntfs_mft_entry_check_for_empty_block(
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_mft_entry_initialize(
     libfsntfs_mft_entry_t **mft_entry,
     libcerror_error_t **error );

int libfsntfs_mft_entry_free(
     libfsntfs_mft_entry_t **mft_entry,
     libcerror_error_t **error );

int libfsntfs_mft_entry_read(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *mft_entry_vector,
     off64_t file_offset,
     uint32_t mft_entry_index,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_mft_entry_read_header(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t mft_entry_index,
     libcerror_error_t **error );

int libfsntfs_mft_entry_read_attributes(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *mft_entry_vector,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_mft_entry_read_attributes_from_attribute_list(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *mft_entry_vector,
     libcdata_array_t *attribute_list,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_mft_entry_read_directory_entries_tree(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcdata_btree_t *directory_entries_tree,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_mft_entry_is_empty(
     libfsntfs_mft_entry_t *mft_entry,
     libcerror_error_t **error );

int libfsntfs_mft_entry_is_allocated(
     libfsntfs_mft_entry_t *mft_entry,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_file_reference(
     libfsntfs_mft_entry_t *mft_entry,
     uint64_t *file_reference,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_base_record_file_reference(
     libfsntfs_mft_entry_t *mft_entry,
     uint64_t *file_reference,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_journal_sequence_number(
     libfsntfs_mft_entry_t *mft_entry,
     uint64_t *journal_sequence_number,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_number_of_attributes(
     libfsntfs_mft_entry_t *mft_entry,
     int *number_of_attributes,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_attribute_by_index(
     libfsntfs_mft_entry_t *mft_entry,
     int attribute_index,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_number_of_alternate_data_attributes(
     libfsntfs_mft_entry_t *mft_entry,
     int *number_of_attributes,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_alternate_data_attribute_by_index(
     libfsntfs_mft_entry_t *mft_entry,
     int attribute_index,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_alternate_data_attribute_by_utf8_name(
     libfsntfs_mft_entry_t *mft_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_alternate_data_attribute_by_utf16_name(
     libfsntfs_mft_entry_t *mft_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_append_index(
     libfsntfs_mft_entry_t *mft_entry,
     const uint8_t *name,
     size_t name_size,
     libfsntfs_index_t **index,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_index_by_name(
     libfsntfs_mft_entry_t *mft_entry,
     const uint8_t *name,
     size_t name_size,
     libfsntfs_index_t **index,
     libcerror_error_t **error );

int libfsntfs_mft_entry_append_attribute(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_attribute_t *attribute,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_mft_entry_append_data_attribute(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_data_attribute_by_name(
     libfsntfs_mft_entry_t *mft_entry,
     const uint8_t *name,
     size_t name_size,
     int *attribute_index,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_append_index_allocation_attribute(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_append_index_root_attribute(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_set_reparse_point_attribute(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_has_directory_entries_index(
     libfsntfs_mft_entry_t *mft_entry,
     libcerror_error_t **error );

int libfsntfs_mft_entry_read_element_data(
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_data_file_index,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_MFT_ENTRY_H ) */

