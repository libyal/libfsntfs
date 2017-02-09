/*
 * Index functions
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

#if !defined( _LIBFSNTFS_INDEX_H )
#define _LIBFSNTFS_INDEX_H

#include <common.h>
#include <types.h>

#include "libfsntfs_attribute.h"
#include "libfsntfs_index_value.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_index libfsntfs_index_t;

struct libfsntfs_index
{
	/* The index entry size
	 */
	uint32_t index_entry_size;

	/* The name
	 */
	const uint8_t *name;

	/* The name size
	 */
	uint16_t name_size;

	/* The attribute type
	 */
	uint32_t attribute_type;

	/* The flags
	 */
	uint32_t flags;

	/* The $INDEX_ROOT attribute
	 */
	libfsntfs_attribute_t *index_root_attribute;

	/* The $INDEX_ALLOCATION attribute
	 */
	libfsntfs_attribute_t *index_allocation_attribute;

	/* The $BITMAP attribute
	 */
	libfsntfs_attribute_t *bitmap_attribute;

	/* The root values array
	 */
	libcdata_array_t *root_values_array;

	/* The index entry vector
	 */
	libfdata_vector_t *index_entry_vector;

	/* The index entry cache
	 */
	libfcache_cache_t *index_entry_cache;

	/* The index value list
	 */
	libfdata_list_t *index_value_list;

	/* The index value cache
	 */
	libfcache_cache_t *index_value_cache;

	/* Value to indicate the index was read
	 */
	uint8_t is_read;
};

int libfsntfs_index_initialize(
     libfsntfs_index_t **index,
     const uint8_t *name,
     size_t name_size,
     libcerror_error_t **error );

int libfsntfs_index_free(
     libfsntfs_index_t **index,
     libcerror_error_t **error );

int libfsntfs_index_set_index_root_attribute(
     libfsntfs_index_t *index,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int libfsntfs_index_set_index_allocation_attribute(
     libfsntfs_index_t *index,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int libfsntfs_index_set_bitmap_attribute(
     libfsntfs_index_t *index,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

int libfsntfs_index_read(
     libfsntfs_index_t *index,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_index_read_root(
     libfsntfs_index_t *index,
     libfsntfs_io_handle_t *io_handle,
     libcerror_error_t **error );

int libfsntfs_index_read_bitmap(
     libfsntfs_index_t *index,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_index_read_sub_nodes(
     libfsntfs_index_t *index,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcdata_array_t *node_values_array,
     libcerror_error_t **error );

int libfsntfs_index_read_index_value_element_data(
     libfsntfs_index_t *index,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *element,
     libfcache_cache_t *cache,
     int element_file_index,
     off64_t index_value_offset,
     size64_t element_size,
     uint32_t index_value_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libfsntfs_index_get_number_of_index_values(
     libfsntfs_index_t *index,
     int *number_of_index_values,
     libcerror_error_t **error );

int libfsntfs_index_get_index_value_by_index(
     libfsntfs_index_t *index,
     libbfio_handle_t *file_io_handle,
     int index_value_entry,
     libfsntfs_index_value_t **index_value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_INDEX_H ) */

