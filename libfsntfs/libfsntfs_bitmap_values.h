/*
 * Bitmap attribute ($BITMAP) values functions
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

#if !defined( _LIBFSNTFS_BITMAP_VALUES_H )
#define _LIBFSNTFS_BITMAP_VALUES_H

#include <common.h>
#include <types.h>

#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_mft_attribute.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_bitmap_values libfsntfs_bitmap_values_t;

struct libfsntfs_bitmap_values
{
	/* The allocated block (range) list
	 */
	libcdata_range_list_t *allocated_block_list;
};

int libfsntfs_bitmap_values_initialize(
     libfsntfs_bitmap_values_t **bitmap_values,
     libcerror_error_t **error );

int libfsntfs_bitmap_values_free(
     libfsntfs_bitmap_values_t **bitmap_values,
     libcerror_error_t **error );

int libfsntfs_bitmap_values_read_data(
     libfsntfs_bitmap_values_t *bitmap_values,
     const uint8_t *data,
     size_t data_size,
     size_t element_data_size,
     off64_t *base_offset,
     libcerror_error_t **error );

int libfsntfs_bitmap_values_read_from_mft_attribute(
     libfsntfs_bitmap_values_t *bitmap_values,
     libfsntfs_mft_attribute_t *mft_attribute,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     size_t element_data_size,
     uint8_t flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_BITMAP_VALUES_H ) */

