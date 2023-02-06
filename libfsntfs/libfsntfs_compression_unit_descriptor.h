/*
 * Compression unit descriptor functions
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

#if !defined( _LIBFSNTFS_COMPRESSION_UNIT_DESCRIPTOR_H )
#define _LIBFSNTFS_COMPRESSION_UNIT_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_compression_unit_descriptor libfsntfs_compression_unit_descriptor_t;

struct libfsntfs_compression_unit_descriptor
{
	/* The data run offset
	 */
	off64_t data_run_offset;

	/* The data offset
	 */
	off64_t data_offset;

	/* The data size
	 */
	size64_t data_size;

	/* The compression unit size
	 */
	size64_t compression_unit_size;

	/* The data range flags
	 */
	uint32_t data_range_flags;

	/* The data stream
	 */
	libfdata_stream_t *data_stream;
};

int libfsntfs_compression_unit_descriptor_initialize(
     libfsntfs_compression_unit_descriptor_t **compression_unit_descriptor,
     libcerror_error_t **error );

int libfsntfs_compression_unit_descriptor_free(
     libfsntfs_compression_unit_descriptor_t **compression_unit_descriptor,
     libcerror_error_t **error );

int libfsntfs_compression_unit_descriptor_append_data_segment(
     libfsntfs_compression_unit_descriptor_t *compression_unit_descriptor,
     off64_t segment_offset,
     size64_t segment_size,
     uint32_t segment_flags,
     libcerror_error_t **error );

#if defined( HAVE_DEBUG_OUTPUT )

int libfsntfs_compression_unit_descriptor_print(
     libfsntfs_compression_unit_descriptor_t *compression_unit_descriptor,
     libfsntfs_io_handle_t *io_handle,
     int compression_unit_descriptor_index,
     libcerror_error_t **error );

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_COMPRESSION_UNIT_DESCRIPTOR_H ) */

