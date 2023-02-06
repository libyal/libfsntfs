/*
 * Index node functions
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

#if !defined( _LIBFSNTFS_INDEX_NODE_H )
#define _LIBFSNTFS_INDEX_NODE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_index_node_header.h"
#include "libfsntfs_index_value.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_index_node libfsntfs_index_node_t;

struct libfsntfs_index_node
{
	/* The node header data offset
	 */
	size_t header_data_offset;

	/* The header
	 */
	libfsntfs_index_node_header_t *header;

	/* The values array
	 */
	libcdata_array_t *values_array;
};

int libfsntfs_index_node_initialize(
     libfsntfs_index_node_t **index_node,
     libcerror_error_t **error );

int libfsntfs_index_node_free(
     libfsntfs_index_node_t **index_node,
     libcerror_error_t **error );

int libfsntfs_index_node_read_header(
     libfsntfs_index_node_t *index_node,
     const uint8_t *data,
     size_t data_size,
     size_t data_offset,
     libcerror_error_t **error );

int libfsntfs_index_node_read_values(
     libfsntfs_index_node_t *index_node,
     off64_t index_value_vcn_offset,
     const uint8_t *data,
     size_t data_size,
     size_t data_offset,
     libcerror_error_t **error );

int libfsntfs_index_node_get_number_of_values(
     libfsntfs_index_node_t *index_node,
     int *number_of_values,
     libcerror_error_t **error );

int libfsntfs_index_node_get_value_by_index(
     libfsntfs_index_node_t *index_node,
     int value_entry,
     libfsntfs_index_value_t **value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_INDEX_NODE_H ) */

