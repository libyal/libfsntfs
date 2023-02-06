/*
 * Cluster block functions
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

#if !defined( _LIBFSNTFS_CLUSTER_BLOCK_H )
#define _LIBFSNTFS_CLUSTER_BLOCK_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_cluster_block libfsntfs_cluster_block_t;

struct libfsntfs_cluster_block
{
	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;
};

int libfsntfs_cluster_block_initialize(
     libfsntfs_cluster_block_t **cluster_block,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_cluster_block_free(
     libfsntfs_cluster_block_t **cluster_block,
     libcerror_error_t **error );

int libfsntfs_cluster_block_clear(
     libfsntfs_cluster_block_t *cluster_block,
     libcerror_error_t **error );

int libfsntfs_cluster_block_read_file_io_handle(
     libfsntfs_cluster_block_t *cluster_block,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_CLUSTER_BLOCK_H ) */

