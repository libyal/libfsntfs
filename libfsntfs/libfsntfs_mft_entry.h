/*
 * Master File Table (MFT) entry functions
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

#if !defined( _LIBFSNTFS_MFT_ENTRY_H )
#define _LIBFSNTFS_MFT_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsntfs_directory_entry.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_mft_attribute_list.h"
#include "libfsntfs_mft_entry_header.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_mft_entry libfsntfs_mft_entry_t;

struct libfsntfs_mft_entry
{
	/* The MFT entry header
	 */
	libfsntfs_mft_entry_header_t *header;

	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;

	/* The index
	 */
	uint32_t index;

	/* The file reference
	 */
	uint64_t file_reference;

	/* The attributes array
	 */
	libcdata_array_t *attributes_array;

	/* The (attribute) list attribute
	 */
	libfsntfs_mft_attribute_t *list_attribute;

	/* The attribute list
	 */
	libfsntfs_mft_attribute_list_t *attribute_list;

	/* The default (nameless) $DATA attribute
	 */
	libfsntfs_mft_attribute_t *data_attribute;

	/* The WofCompressedData $DATA attribute
	 */
	libfsntfs_mft_attribute_t *wof_compressed_data_attribute;

	/* The alternate data attributes array
	 */
	libcdata_array_t *alternate_data_attributes_array;

	/* The index of the (first) file name attribute or -1 if not set
	 */
	int file_name_attribute_index;

	/* The index of the reparse point attribute or -1 if not set
	 */
	int reparse_point_attribute_index;

	/* The index of the security descriptor attribute or -1 if not set
	 */
	int security_descriptor_attribute_index;

	/* The index of the standard information attribute or -1 if not set
	 */
	int standard_information_attribute_index;

	/* The index of the volume information attribute or -1 if not set
	 */
	int volume_information_attribute_index;

	/* The index of the volume name attribute or -1 if not set
	 */
	int volume_name_attribute_index;

	/* Value to indicate the MFT entry is empty
	 */
	uint8_t is_empty;

	/* Value to indicate the MFT entry is corrupted
	 */
	uint8_t is_corrupted;

	/* Value to indicate the MFT entry has an $I30 index
	 */
	uint8_t has_i30_index;

	/* Value to indicate the attributes have been read
	 */
	uint8_t attributes_read;
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

int libfsntfs_mft_entry_read_data(
     libfsntfs_mft_entry_t *mft_entry,
     uint8_t *data,
     size_t data_size,
     uint32_t mft_entry_index,
     libcerror_error_t **error );

int libfsntfs_mft_entry_read_file_io_handle(
     libfsntfs_mft_entry_t *mft_entry,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t mft_entry_size,
     uint32_t mft_entry_index,
     libcerror_error_t **error );

int libfsntfs_mft_entry_read_attributes_data(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_mft_entry_read_attributes(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *mft_entry_vector,
     libcdata_btree_t *attribute_list_tree,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_mft_entry_read_attribute_list(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libfsntfs_mft_entry_read_attribute_list_data_mft_entry_by_index(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *mft_entry_vector,
     libfcache_cache_t *mft_entry_cache,
     uint64_t file_reference,
     libcerror_error_t **error );

int libfsntfs_mft_entry_read_attribute_list_data_mft_entries(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_mft_attribute_list_t *attribute_list,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *mft_entry_vector,
     libcerror_error_t **error );

int libfsntfs_mft_entry_is_empty(
     libfsntfs_mft_entry_t *mft_entry,
     libcerror_error_t **error );

int libfsntfs_mft_entry_is_allocated(
     libfsntfs_mft_entry_t *mft_entry,
     libcerror_error_t **error );

int libfsntfs_mft_entry_is_corrupted(
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
     libfsntfs_mft_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_standard_information_attribute(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_mft_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_volume_information_attribute(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_mft_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_volume_name_attribute(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_mft_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_set_attribute_by_index(
     libfsntfs_mft_entry_t *mft_entry,
     int attribute_index,
     libfsntfs_mft_attribute_t *attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_number_of_alternate_data_attributes(
     libfsntfs_mft_entry_t *mft_entry,
     int *number_of_attributes,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_alternate_data_attribute_by_index(
     libfsntfs_mft_entry_t *mft_entry,
     int attribute_index,
     libfsntfs_mft_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_alternate_data_attribute_by_utf8_name(
     libfsntfs_mft_entry_t *mft_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_mft_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_alternate_data_attribute_by_utf16_name(
     libfsntfs_mft_entry_t *mft_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_mft_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_set_attribute_helper_values(
     libfsntfs_mft_entry_t *mft_entry,
     int attribute_index,
     libfsntfs_mft_attribute_t *attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_set_data_attribute_helper_values(
     libfsntfs_mft_entry_t *mft_entry,
     libfsntfs_mft_attribute_t *data_attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_get_data_attribute_by_utf8_name(
     libfsntfs_mft_entry_t *mft_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int *attribute_index,
     libfsntfs_mft_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_mft_entry_has_directory_entries_index(
     libfsntfs_mft_entry_t *mft_entry,
     libcerror_error_t **error );

int libfsntfs_mft_entry_read_element_data(
     intptr_t *data_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfdata_cache_t *cache,
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

