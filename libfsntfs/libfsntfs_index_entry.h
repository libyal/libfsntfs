/*
 * Index entry functions
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

#if !defined( _LIBFSNTFS_INDEX_ENTRY_H )
#define _LIBFSNTFS_INDEX_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_index_entry libfsntfs_index_entry_t;

struct libfsntfs_index_entry
{
	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;

	/* The values data
	 */
	uint8_t *values_data;

	/* The values data offset
	 */
	size_t values_data_offset;

	/* The values data size
	 */
	size_t values_data_size;
};

int libfsntfs_index_entry_initialize(
     libfsntfs_index_entry_t **index_entry,
     libcerror_error_t **error );

int libfsntfs_index_entry_free(
     libfsntfs_index_entry_t **index_entry,
     libcerror_error_t **error );

int libfsntfs_index_entry_read(
     libfsntfs_index_entry_t *index_entry,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t index_entry_size,
     uint32_t index_entry_index,
     libcerror_error_t **error );

int libfsntfs_index_entry_read_index_values(
     libfsntfs_index_entry_t *index_entry,
     off64_t index_entry_offset,
     int *index_value_entry,
     libcdata_array_t **index_values_array,
     libcerror_error_t **error );

int libfsntfs_index_entry_read_element_data(
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_file_index,
     off64_t index_entry_offset,
     size64_t index_entry_size,
     uint32_t element_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_INDEX_ENTRY_H ) */

