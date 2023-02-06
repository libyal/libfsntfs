/*
 * The compression unit data handle functions
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

#if !defined( _LIBFSNTFS_COMPRESSION_UNIT_DATA_HANDLE_H )
#define _LIBFSNTFS_COMPRESSION_UNIT_DATA_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_compression_unit_descriptor.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_mft_attribute.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_compression_unit_data_handle libfsntfs_compression_unit_data_handle_t;

struct libfsntfs_compression_unit_data_handle
{
	/* The compression unit descriptors array
	 */
	libcdata_array_t *descriptors_array;

	/* The compression unit size
	 */
	size_t compression_unit_size;
};

int libfsntfs_compression_unit_data_handle_initialize(
     libfsntfs_compression_unit_data_handle_t **data_handle,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error );

int libfsntfs_compression_unit_data_handle_free(
     libfsntfs_compression_unit_data_handle_t **data_handle,
     libcerror_error_t **error );

int libfsntfs_compression_unit_data_handle_get_number_of_descriptors(
     libfsntfs_compression_unit_data_handle_t *data_handle,
     int *number_of_descriptors,
     libcerror_error_t **error );

int libfsntfs_compression_unit_data_handle_get_descriptor_by_index(
     libfsntfs_compression_unit_data_handle_t *data_handle,
     int descriptor_index,
     libfsntfs_compression_unit_descriptor_t **descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_COMPRESSION_UNIT_DATA_HANDLE_H ) */

