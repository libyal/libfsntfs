/*
 * Master File Table (MFT) attribute functions
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

#if !defined( _LIBFSNTFS_MFT_ATTRIBUTE_H )
#define _LIBFSNTFS_MFT_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_data_run.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_mft_attribute libfsntfs_mft_attribute_t;

struct libfsntfs_mft_attribute
{
	/* The type
	 */
	uint32_t type;

	/* The size
	 */
	uint32_t size;

	/* The non-resident flag
	 */
	uint8_t non_resident_flag;

	/* The name size
	 */
	uint16_t name_size;

	/* The data flags
	 */
	uint16_t data_flags;

	/* The identifier
	 */
	uint16_t identifier;

	/* The data size
	 */
	uint64_t data_size;

	/* The data offset
	 */
	uint16_t data_offset;

	/* The data first VCN
	 */
	uint64_t data_first_vcn;

	/* The data last VCN
	 */
	uint64_t data_last_vcn;

	/* The compression unit size
	 */
	size_t compression_unit_size;

	/* The allocated data size
	 */
	uint64_t allocated_data_size;

	/* The valid data size
	 */
	uint64_t valid_data_size;

	/* The name
	 */
	uint8_t *name;

	/* The data
	 */
	uint8_t *data;

	/* The data runs array
	 */
	libcdata_array_t *data_runs_array;

	/* The next attribute in an attribute chain
	 */
	libfsntfs_mft_attribute_t *next_attribute;
};

int libfsntfs_mft_attribute_initialize(
     libfsntfs_mft_attribute_t **mft_attribute,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_free(
     libfsntfs_mft_attribute_t **mft_attribute,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_clone(
     libfsntfs_mft_attribute_t **destination_mft_attribute,
     libfsntfs_mft_attribute_t *source_mft_attribute,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_read_data(
     libfsntfs_mft_attribute_t *mft_attribute,
     libfsntfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_data_is_resident(
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_type(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint32_t *type,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_data_flags(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint16_t *data_flags,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_data_size(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint64_t *data_size,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_data_vcn_range(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint64_t *data_first_vcn,
     uint64_t *data_last_vcn,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_allocated_data_size(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint64_t *allocated_data_size,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_valid_data_size(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint64_t *valid_data_size,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_utf8_name_size(
     libfsntfs_mft_attribute_t *mft_attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_utf8_name(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_utf16_name_size(
     libfsntfs_mft_attribute_t *mft_attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_utf16_name(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_compare_name_with_utf8_string(
     libfsntfs_mft_attribute_t *mft_attribute,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_compare_name_with_utf16_string(
     libfsntfs_mft_attribute_t *mft_attribute,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_compression_unit_size(
     libfsntfs_mft_attribute_t *mft_attribute,
     size_t *compression_unit_size,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_resident_data(
     libfsntfs_mft_attribute_t *mft_attribute,
     uint8_t **resident_data,
     size_t *resident_data_size,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_number_of_data_runs(
     libfsntfs_mft_attribute_t *mft_attribute,
     int *number_of_data_runs,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_data_run_by_index(
     libfsntfs_mft_attribute_t *mft_attribute,
     int data_run_index,
     libfsntfs_data_run_t **data_run,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_data_extents_array(
     libfsntfs_mft_attribute_t *mft_attribute,
     libfsntfs_io_handle_t *io_handle,
     libcdata_array_t **data_extents_array,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_get_next_attribute(
     libfsntfs_mft_attribute_t *mft_attribute,
     libfsntfs_mft_attribute_t **next_attribute,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_append_to_chain(
     libfsntfs_mft_attribute_t **first_attribute,
     libfsntfs_mft_attribute_t *additional_attribute,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_MFT_ATTRIBUTE_H ) */

