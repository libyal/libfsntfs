/*
 * Attribute functions
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

#if !defined( _LIBFSNTFS_INTERNAL_ATTRIBUTE_H )
#define _LIBFSNTFS_INTERNAL_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_data_run.h"
#include "libfsntfs_extern.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_internal_attribute libfsntfs_internal_attribute_t;

struct libfsntfs_internal_attribute
{
	/* The size
	 */
	uint32_t size;

	/* The type
	 */
	uint32_t type;

	/* Value to indicate the attribute is resident
	 */
	uint8_t is_resident;

	/* The identifier
	 */
	uint16_t identifier;

	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	uint16_t name_size;

	/* The data first VCN
	 */
	uint64_t data_first_vcn;

	/* The data last VCN
	 */
	uint64_t data_last_vcn;

	/* The data flags
	 */
	uint16_t data_flags;

	/* The compression unit size
	 */
	size_t compression_unit_size;

	/* The allocated data size
	 */
	size64_t allocated_data_size;

	/* The valid data size
	 */
	size64_t valid_data_size;

	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size64_t data_size;

	/* The data runs array
	 */
	libcdata_array_t *data_runs_array;

	/* The file reference
	 */
	uint64_t file_reference;

	/* The item value
	 */
        intptr_t *value;

	/* The item free value function
	 */
	int (*free_value)(
	       intptr_t **value,
	       libcerror_error_t **error );

	/* The next attribute in the chain
	 */
	libfsntfs_attribute_t *next_attribute;
};

int libfsntfs_attribute_initialize(
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_free(
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_internal_attribute_free(
     libfsntfs_internal_attribute_t **internal_attribute,
     libcerror_error_t **error );

int libfsntfs_attribute_compare_by_file_reference(
     libfsntfs_internal_attribute_t *first_attribute,
     libfsntfs_internal_attribute_t *second_attribute,
     libcerror_error_t **error );

ssize_t libfsntfs_attribute_read_from_mft(
         libfsntfs_attribute_t *attribute,
         libfsntfs_io_handle_t *io_handle,
         uint8_t *mft_entry_data,
         size_t mft_entry_data_size,
         size_t mft_attribute_data_offset,
         uint8_t flags,
         libcerror_error_t **error );

ssize_t libfsntfs_attribute_read_from_list(
         libfsntfs_attribute_t *attribute,
         const uint8_t *data,
         size_t data_size,
         size_t data_offset,
         libcerror_error_t **error );

int libfsntfs_attribute_read_value(
     libfsntfs_attribute_t *attribute,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t flags,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_type(
     libfsntfs_attribute_t *attribute,
     uint32_t *type,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_data_flags(
     libfsntfs_attribute_t *attribute,
     uint16_t *data_flags,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_value(
     libfsntfs_attribute_t *attribute,
     intptr_t **value,
     libcerror_error_t **error );

int libfsntfs_attribute_has_name(
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_utf8_name_size(
     libfsntfs_attribute_t *attribute,
     size_t *utf8_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_utf8_name(
     libfsntfs_attribute_t *attribute,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_utf16_name_size(
     libfsntfs_attribute_t *attribute,
     size_t *utf16_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_utf16_name(
     libfsntfs_attribute_t *attribute,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error );

int libfsntfs_attribute_compare_name_with_utf8_string(
     libfsntfs_attribute_t *attribute,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

int libfsntfs_attribute_compare_name_with_utf16_string(
     libfsntfs_attribute_t *attribute,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_data_vcn_range(
     libfsntfs_attribute_t *attribute,
     uint64_t *data_first_vcn,
     uint64_t *data_last_vcn,
     libcerror_error_t **error );

int libfsntfs_attribute_get_compression_unit_size(
     libfsntfs_attribute_t *attribute,
     size_t *compression_unit_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_file_reference(
     libfsntfs_attribute_t *attribute,
     uint64_t *mft_entry_index,
     uint16_t *sequence_number,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_data_size(
     libfsntfs_attribute_t *attribute,
     size64_t *data_size,
     libcerror_error_t **error );

int libfsntfs_attribute_get_data(
     libfsntfs_attribute_t *attribute,
     uint8_t **data,
     size64_t *data_size,
     libcerror_error_t **error );

ssize_t libfsntfs_attribute_copy_data(
         libfsntfs_attribute_t *attribute,
         uint8_t *buffer,
         size_t buffer_size,
         off64_t data_offset,
         libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_attribute_get_valid_data_size(
     libfsntfs_attribute_t *attribute,
     size64_t *valid_data_size,
     libcerror_error_t **error );

int libfsntfs_attribute_get_number_of_data_runs(
     libfsntfs_attribute_t *attribute,
     int *number_of_data_runs,
     libcerror_error_t **error );

int libfsntfs_attribute_get_data_run_by_index(
     libfsntfs_attribute_t *attribute,
     int data_run_index,
     libfsntfs_data_run_t **data_run,
     libcerror_error_t **error );

int libfsntfs_attribute_get_chained_attribute(
     libfsntfs_attribute_t *attribute,
     libfsntfs_attribute_t **chained_attribute,
     libcerror_error_t **error );

int libfsntfs_attribute_append_to_chain(
     libfsntfs_attribute_t **attribute,
     libfsntfs_attribute_t *chained_attribute,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_INTERNAL_ATTRIBUTE_H ) */

