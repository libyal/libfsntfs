/*
 * Attribute list attribute ($ATTRIBUTE_LIST) functions
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

#if !defined( _LIBFSNTFS_MFT_ATTRIBUTE_LIST_H )
#define _LIBFSNTFS_MFT_ATTRIBUTE_LIST_H

#include <common.h>
#include <types.h>

#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_mft_attribute_list_entry.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_mft_attribute_list libfsntfs_mft_attribute_list_t;

struct libfsntfs_mft_attribute_list
{
	/* The base record file reference
	 */
	uint64_t base_record_file_reference;

	/* The attribute list entries array
	 */
	libcdata_array_t *entries_array;

	/* The attribute list data file references attary
	 */
	libcdata_array_t *file_references_array;
};

int libfsntfs_mft_attribute_list_initialize(
     libfsntfs_mft_attribute_list_t **attribute_list,
     uint64_t base_record_file_reference,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_list_free(
     libfsntfs_mft_attribute_list_t **attribute_list,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_list_file_reference_free(
     uint64_t **file_reference,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_list_read_data(
     libfsntfs_mft_attribute_list_t *attribute_list,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_list_read_from_attribute(
     libfsntfs_mft_attribute_list_t *attribute_list,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_attribute_t *list_attribute,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_list_get_number_of_entries(
     libfsntfs_mft_attribute_list_t *attribute_list,
     int *number_of_entries,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_list_get_entry_by_index(
     libfsntfs_mft_attribute_list_t *attribute_list,
     int entry_index,
     libfsntfs_mft_attribute_list_entry_t **mft_attribute_list_entry,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_list_compare_by_base_record_file_reference(
     libfsntfs_mft_attribute_list_t *first_attribute_list,
     libfsntfs_mft_attribute_list_t *second_attribute_list,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_list_get_number_of_file_references(
     libfsntfs_mft_attribute_list_t *attribute_list,
     int *number_of_file_references,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_list_get_file_reference_by_index(
     libfsntfs_mft_attribute_list_t *attribute_list,
     int file_reference_index,
     uint64_t *file_reference,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_list_compare_file_reference(
     uint64_t *first_file_reference,
     uint64_t *second_file_reference,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_list_insert_file_reference(
     libfsntfs_mft_attribute_list_t *attribute_list,
     uint64_t file_reference,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_MFT_ATTRIBUTE_LIST_H ) */

