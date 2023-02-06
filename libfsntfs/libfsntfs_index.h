/*
 * Index functions
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

#if !defined( _LIBFSNTFS_INDEX_H )
#define _LIBFSNTFS_INDEX_H

#include <common.h>
#include <types.h>

#include "libfsntfs_bitmap_values.h"
#include "libfsntfs_index_node.h"
#include "libfsntfs_index_root_header.h"
#include "libfsntfs_index_value.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_mft_entry.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_index libfsntfs_index_t;

struct libfsntfs_index
{
	/* The IO handle
	 */
	libfsntfs_io_handle_t *io_handle;

	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	size_t name_size;

	/* The flags
	 */
	uint32_t flags;

	/* The root header
	 */
	libfsntfs_index_root_header_t *root_header;

	/* The root node
	 */
	libfsntfs_index_node_t *root_node;

	/* The index entry vector
	 */
	libfdata_vector_t *index_entry_vector;

	/* The index node cache
	 */
	libfcache_cache_t *index_node_cache;

	/* The bitmap values
	 */
	libfsntfs_bitmap_values_t *bitmap_values;
};

int libfsntfs_index_initialize(
     libfsntfs_index_t **index,
     libfsntfs_io_handle_t *io_handle,
     const uint8_t *name,
     size_t name_size,
     libcerror_error_t **error );

int libfsntfs_index_free(
     libfsntfs_index_t **index,
     libcerror_error_t **error );

int libfsntfs_index_read(
     libfsntfs_index_t *index,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_entry_t *mft_entry,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_index_read_root_header(
     libfsntfs_index_t *index,
     libfsntfs_mft_attribute_t *index_root_attribute,
     libcerror_error_t **error );

int libfsntfs_index_read_root_node(
     libfsntfs_index_t *index,
     libfsntfs_mft_attribute_t *index_root_attribute,
     libcerror_error_t **error );

int libfsntfs_index_read_bitmap(
     libfsntfs_index_t *index,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_attribute_t *bitmap_attribute,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_index_get_index_entry_size(
     libfsntfs_index_t *index,
     uint32_t *index_entry_size,
     libcerror_error_t **error );

int libfsntfs_index_get_attribute_type(
     libfsntfs_index_t *index,
     uint32_t *attribute_type,
     libcerror_error_t **error );

int libfsntfs_index_get_collation_type(
     libfsntfs_index_t *index,
     uint32_t *collation_type,
     libcerror_error_t **error );

int libfsntfs_index_sub_node_is_allocated(
     libfsntfs_index_t *index,
     int sub_node_vcn,
     libcerror_error_t **error );

int libfsntfs_index_get_sub_node(
     libfsntfs_index_t *index,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *index_node_cache,
     off64_t index_entry_offset,
     int sub_node_vcn,
     libfsntfs_index_node_t **index_node,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_INDEX_H ) */

