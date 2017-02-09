/*
 * Index node functions
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

#if !defined( _LIBFSNTFS_INDEX_NODE_H )
#define _LIBFSNTFS_INDEX_NODE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_index_node libfsntfs_index_node_t;

struct libfsntfs_index_node
{
	/* The size
	 */
	uint32_t size;

	/* The index values offset
	 */
	uint32_t index_values_offset;

	/* The flags
	 */
	uint32_t flags;
};

int libfsntfs_index_node_initialize(
     libfsntfs_index_node_t **index_node,
     libcerror_error_t **error );

int libfsntfs_index_node_free(
     libfsntfs_index_node_t **index_node,
     libcerror_error_t **error );

size_t libfsntfs_index_node_read(
        libfsntfs_index_node_t *index_node,
        uint8_t *index_node_data,
        size_t index_node_data_size,
        size_t index_node_data_offset,
        libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_INDEX_NODE_H ) */

