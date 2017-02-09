/*
 * Compressed block descriptor functions
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

#if !defined( _LIBFSNTFS_COMPRESSED_BLOCK_DESCRIPTOR_H )
#define _LIBFSNTFS_COMPRESSED_BLOCK_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_compressed_block_descriptor libfsntfs_compressed_block_descriptor_t;

struct libfsntfs_compressed_block_descriptor
{
	/* The data stream
	 */
	libfdata_stream_t *data_stream;

	/* The data range flags
	 */
	uint32_t data_range_flags;

	/* The data size
	 */
	size_t data_size;
};

int libfsntfs_compressed_block_descriptor_initialize(
     libfsntfs_compressed_block_descriptor_t **compressed_block_descriptor,
     libcerror_error_t **error );

int libfsntfs_compressed_block_descriptor_free(
     libfsntfs_compressed_block_descriptor_t **compressed_block_descriptor,
     libcerror_error_t **error );

int libfsntfs_compressed_block_descriptor_append_data_segment(
     libfsntfs_compressed_block_descriptor_t *compressed_block_descriptor,
     off64_t segment_offset,
     size64_t segment_size,
     uint32_t segment_flags,
     libcerror_error_t **error );

ssize_t libfsntfs_compressed_block_descriptor_read_segment_data(
         intptr_t *data_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags,
         libcerror_error_t **error );

off64_t libfsntfs_compressed_block_descriptor_seek_segment_offset(
         intptr_t *data_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         off64_t segment_offset,
         libcerror_error_t **error );

#if defined( HAVE_DEBUG_OUTPUT )

int libfsntfs_compressed_block_descriptor_print(
     libfsntfs_compressed_block_descriptor_t *compressed_block_descriptor,
     libfsntfs_io_handle_t *io_handle,
     int compressed_block_descriptor_index,
     libcerror_error_t **error );

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_COMPRESSED_BLOCK_DESCRIPTOR_H ) */

