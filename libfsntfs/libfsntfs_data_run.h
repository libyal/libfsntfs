/*
 * Data run functions
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

#if !defined( _LIBFSNTFS_DATA_RUN_H )
#define _LIBFSNTFS_DATA_RUN_H

#include <common.h>
#include <types.h>

#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_data_run libfsntfs_data_run_t;

struct libfsntfs_data_run
{
	/* The cluster block number
	 */
	uint64_t cluster_block_number;

	/* The start offset
	 */
	off64_t start_offset;

	/* The size
	 */
	size64_t size;

	/* The range flags
	 */
	uint32_t range_flags;
};

int libfsntfs_data_run_initialize(
     libfsntfs_data_run_t **data_run,
     libcerror_error_t **error );

int libfsntfs_data_run_free(
     libfsntfs_data_run_t **data_run,
     libcerror_error_t **error );

int libfsntfs_data_run_clone(
     libfsntfs_data_run_t **destination_data_run,
     libfsntfs_data_run_t *source_data_run,
     libcerror_error_t **error );

ssize_t libfsntfs_data_run_read_data(
         libfsntfs_data_run_t *data_run,
         libfsntfs_io_handle_t *io_handle,
         const uint8_t *data,
         size_t data_size,
         uint64_t last_cluster_block_number,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_DATA_RUN_H ) */

