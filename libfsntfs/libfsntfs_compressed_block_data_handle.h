/*
 * The compressed block data handle functions
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

#if !defined( _LIBFSNTFS_COMPRESSED_BLOCK_DATA_HANDLE_H )
#define _LIBFSNTFS_COMPRESSED_BLOCK_DATA_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_attribute.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_compressed_block_data_handle libfsntfs_compressed_block_data_handle_t;

struct libfsntfs_compressed_block_data_handle
{
	/* The current offset
	 */
	off64_t current_offset;

	/* The data size
	 */
	size64_t data_size;

	/* The compressed block vector
	 */
	libfdata_vector_t *compressed_block_vector;

	/* The compressed block cache
	 */
	libfcache_cache_t *compressed_block_cache;
};

int libfsntfs_compressed_block_data_handle_initialize(
     libfsntfs_compressed_block_data_handle_t **data_handle,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error );

int libfsntfs_compressed_block_data_handle_free(
     libfsntfs_compressed_block_data_handle_t **data_handle,
     libcerror_error_t **error );

ssize_t libfsntfs_compressed_block_data_handle_read_segment_data(
         libfsntfs_compressed_block_data_handle_t *data_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags,
         libcerror_error_t **error );

off64_t libfsntfs_compressed_block_data_handle_seek_segment_offset(
         libfsntfs_compressed_block_data_handle_t *data_handle,
         intptr_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         off64_t segment_offset,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_COMPRESSED_BLOCK_DATA_HANDLE_H ) */

