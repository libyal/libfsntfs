/*
 * The cluster block stream functions
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

#if !defined( _LIBFSNTFS_CLUSTER_BLOCK_STREAM_H )
#define _LIBFSNTFS_CLUSTER_BLOCK_STREAM_H

#include <common.h>
#include <types.h>

#include "libfsntfs_attribute.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_cluster_block_stream_data_handle libfsntfs_cluster_block_stream_data_handle_t;

struct libfsntfs_cluster_block_stream_data_handle
{
	/* The IO handle
	 */
	libfsntfs_io_handle_t *io_handle;

	/* The current segment offset
	 */
	off64_t current_segment_offset;

	/* The data size
	 */
	size64_t data_size;

	/* The compression unit size
	 */
	size_t compression_unit_size;

	/* The data flags
	 */
	uint16_t data_flags;

	/* The resident data
	 */
	const uint8_t *resident_data;

	/* The compressed block descriptors array
	 */
	libcdata_array_t *compressed_block_descriptors_array;

	/* The compressed block vector
	 */
	libfdata_vector_t *compressed_block_vector;

	/* The cache
	 */
	libfcache_cache_t *cache;
};

int libfsntfs_cluster_block_stream_data_handle_initialize(
     libfsntfs_cluster_block_stream_data_handle_t **data_handle,
     libfsntfs_io_handle_t *io_handle,
     size64_t data_size,
     const uint8_t *resident_data,
     size_t compression_unit_size,
     uint16_t data_flags,
     libcerror_error_t **error );

int libfsntfs_cluster_block_stream_data_handle_free(
     libfsntfs_cluster_block_stream_data_handle_t **data_handle,
     libcerror_error_t **error );

int libfsntfs_cluster_block_stream_data_handle_clone(
     libfsntfs_cluster_block_stream_data_handle_t **destination_data_handle,
     libfsntfs_cluster_block_stream_data_handle_t *source_data_handle,
     libcerror_error_t **error );

ssize_t libfsntfs_cluster_block_stream_data_handle_read_segment_data(
         libfsntfs_cluster_block_stream_data_handle_t *data_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags,
         libcerror_error_t **error );

off64_t libfsntfs_cluster_block_stream_data_handle_seek_segment_offset(
         libfsntfs_cluster_block_stream_data_handle_t *data_handle,
         intptr_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         off64_t segment_offset,
         libcerror_error_t **error );

int libfsntfs_cluster_block_stream_initialize(
     libfdata_stream_t **cluster_block_stream,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_attribute_t *attribute,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_CLUSTER_BLOCK_STREAM_H ) */

