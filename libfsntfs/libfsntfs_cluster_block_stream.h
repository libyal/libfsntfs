/*
 * Cluster block stream functions
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

#if !defined( _LIBFSNTFS_CLUSTER_BLOCK_STREAM_H )
#define _LIBFSNTFS_CLUSTER_BLOCK_STREAM_H

#include <common.h>
#include <types.h>

#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_attribute.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfsntfs_cluster_block_stream_initialize_from_data(
     libfdata_stream_t **cluster_block_stream,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_cluster_block_stream_initialize_from_data_runs(
     libfdata_stream_t **cluster_block_stream,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_mft_attribute_t *data_attribute,
     size64_t data_size,
     libcerror_error_t **error );

int libfsntfs_cluster_block_stream_initialize_from_compressed_data_runs(
     libfdata_stream_t **cluster_block_stream,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_mft_attribute_t *data_attribute,
     size64_t data_size,
     libcerror_error_t **error );

int libfsntfs_cluster_block_stream_initialize_from_compressed_stream(
     libfdata_stream_t **cluster_block_stream,
     libfdata_stream_t *compressed_data_stream,
     libfsntfs_mft_attribute_t *data_attribute,
     uint32_t compression_method,
     libcerror_error_t **error );

int libfsntfs_cluster_block_stream_initialize(
     libfdata_stream_t **cluster_block_stream,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_mft_attribute_t *data_attribute,
     libfsntfs_mft_attribute_t *wof_compressed_data_attribute,
     uint32_t compression_method,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_CLUSTER_BLOCK_STREAM_H ) */

