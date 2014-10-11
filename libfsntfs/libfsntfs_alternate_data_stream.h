/*
 * Alternate data stream functions
 *
 * Copyright (c) 2010-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSNTFS_INTERNAL_ALTERNATE_DATA_STREAM_H )
#define _LIBFSNTFS_INTERNAL_ALTERNATE_DATA_STREAM_H

#include <common.h>
#include <types.h>

#include "libfsntfs_attribute.h"
#include "libfsntfs_extern.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_internal_alternate_data_stream libfsntfs_internal_alternate_data_stream_t;

struct libfsntfs_internal_alternate_data_stream
{
	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The $DATA attribute
	 */
	libfsntfs_attribute_t *data_attribute;

	/* The $DATA attribute cluster block stream
	 */
	libfdata_stream_t *data_cluster_block_stream;
};

int libfsntfs_alternate_data_stream_initialize(
     libfsntfs_alternate_data_stream_t **alternate_data_stream,
     libbfio_handle_t *file_io_handle,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_attribute_t *data_attribute,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_alternate_data_stream_free(
     libfsntfs_alternate_data_stream_t **alternate_data_stream,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_alternate_data_stream_get_utf8_name_size(
     libfsntfs_alternate_data_stream_t *alternate_data_stream,
     size_t *utf8_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_alternate_data_stream_get_utf8_name(
     libfsntfs_alternate_data_stream_t *alternate_data_stream,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_alternate_data_stream_get_utf16_name_size(
     libfsntfs_alternate_data_stream_t *alternate_data_stream,
     size_t *utf16_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_alternate_data_stream_get_utf16_name(
     libfsntfs_alternate_data_stream_t *alternate_data_stream,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
ssize_t libfsntfs_alternate_data_stream_read_buffer(
         libfsntfs_alternate_data_stream_t *alternate_data_stream,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBFSNTFS_EXTERN \
ssize_t libfsntfs_alternate_data_stream_read_buffer_at_offset(
         libfsntfs_alternate_data_stream_t *alternate_data_stream,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBFSNTFS_EXTERN \
off64_t libfsntfs_alternate_data_stream_seek_offset(
         libfsntfs_alternate_data_stream_t *alternate_data_stream,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_alternate_data_stream_get_offset(
     libfsntfs_alternate_data_stream_t *alternate_data_stream,
     off64_t *offset,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_alternate_data_stream_get_size(
     libfsntfs_alternate_data_stream_t *alternate_data_stream,
     size64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

