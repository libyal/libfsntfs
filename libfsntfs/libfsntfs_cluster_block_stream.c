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

#include <common.h>
#include <types.h>

#include "libfsntfs_buffer_data_handle.h"
#include "libfsntfs_cluster_block_data.h"
#include "libfsntfs_cluster_block_stream.h"
#include "libfsntfs_compressed_block_data_handle.h"
#include "libfsntfs_compressed_data_handle.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_attribute.h"

/* Creates cluster block stream from a buffer of data
 * Make sure the value cluster_block_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_stream_initialize_from_data(
     libfdata_stream_t **cluster_block_stream,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfdata_stream_t *safe_data_stream         = NULL;
	libfsntfs_buffer_data_handle_t *data_handle = NULL;
	static char *function                       = "libfsntfs_cluster_block_stream_initialize_from_data";
	int segment_index                           = 0;

	if( cluster_block_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cluster block stream.",
		 function );

		return( -1 );
	}
	if( libfsntfs_buffer_data_handle_initialize(
	     &data_handle,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create buffer data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     &safe_data_stream,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_buffer_data_handle_free,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_buffer_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfsntfs_buffer_data_handle_seek_segment_offset,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data stream.",
		 function );

		goto on_error;
	}
	data_handle = NULL;

	if( libfdata_stream_append_segment(
	     safe_data_stream,
	     &segment_index,
	     0,
	     0,
	     (size64_t) data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append data stream segment.",
		 function );

		goto on_error;
	}
	*cluster_block_stream = safe_data_stream;

	return( 1 );

on_error:
	if( safe_data_stream != NULL )
	{
		libfdata_stream_free(
		 &safe_data_stream,
		 NULL );
	}
	if( data_handle != NULL )
	{
		libfsntfs_buffer_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

/* Creates cluster block stream from data runs
 * Make sure the value data_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_stream_initialize_from_data_runs(
     libfdata_stream_t **cluster_block_stream,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_mft_attribute_t *data_attribute,
     size64_t data_size,
     libcerror_error_t **error )
{
	libfdata_stream_t *safe_data_stream          = NULL;
	libfsntfs_data_run_t *data_run               = NULL;
	static char *function                        = "libfsntfs_cluster_block_stream_initialize_from_data_runs";
	size64_t attribute_data_vcn_size             = 0;
	size64_t calculated_allocated_data_size      = 0;
	size64_t data_segment_size                   = 0;
	size64_t stored_allocated_data_size          = 0;
	size64_t valid_data_size                     = 0;
	off64_t attribute_data_vcn_offset            = 0;
	off64_t calculated_attribute_data_vcn_offset = 0;
	off64_t data_segment_offset                  = 0;
	uint16_t attribute_data_flags                = 0;
	int attribute_index                          = 0;
	int data_run_index                           = 0;
	int number_of_data_runs                      = 0;
	int segment_index                            = 0;

	if( cluster_block_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle->cluster_block_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid IO handle - cluster block size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_attribute_get_valid_data_size(
	     data_attribute,
	     &valid_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve valid data size.",
		 function );

		goto on_error;
	}
	if( libfsntfs_mft_attribute_get_data_flags(
	     data_attribute,
	     &attribute_data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute data flags.",
		 function );

		goto on_error;
	}
	if( ( attribute_data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compressed attribute data.",
		 function );

		goto on_error;
	}
	if( libfsntfs_mft_attribute_get_allocated_data_size(
	     data_attribute,
	     &stored_allocated_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute allocated data size.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     &safe_data_stream,
	     NULL,
	     NULL,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_cluster_block_data_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfsntfs_cluster_block_data_seek_segment_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data stream.",
		 function );

		goto on_error;
	}
	while( data_attribute != NULL )
	{
		if( libfsntfs_mft_attribute_get_data_vcn_range(
		     data_attribute,
		     (uint64_t *) &attribute_data_vcn_offset,
		     (uint64_t *) &attribute_data_vcn_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute data VCN range.",
			 function );

			goto on_error;
		}
		if( attribute_data_vcn_size != 0xffffffffffffffffULL )
		{
			if( (uint64_t) attribute_data_vcn_offset > (uint64_t) ( ( INT64_MAX / io_handle->cluster_block_size ) - 1 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid attribute data first VCN value out of bounds.",
				 function );

				goto on_error;
			}
			if( attribute_data_vcn_size > (size64_t) ( ( INT64_MAX / io_handle->cluster_block_size ) - 1 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid attribute data last VCN value out of bounds.",
				 function );

				goto on_error;
			}
			if( attribute_data_vcn_offset > (off64_t) attribute_data_vcn_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid attribute data first VCN value exceeds last VCN value.",
				 function );

				goto on_error;
			}
			attribute_data_vcn_size   += 1;
			attribute_data_vcn_size   -= attribute_data_vcn_offset;
			attribute_data_vcn_offset *= io_handle->cluster_block_size;
			attribute_data_vcn_size   *= io_handle->cluster_block_size;

			if( ( calculated_attribute_data_vcn_offset != 0 )
			 && ( calculated_attribute_data_vcn_offset != attribute_data_vcn_offset ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid attribute data VCN offset value out of bounds.",
				 function );

				goto on_error;
			}
			calculated_attribute_data_vcn_offset = attribute_data_vcn_offset + (off64_t) attribute_data_vcn_size;
		}
		if( libfsntfs_mft_attribute_get_number_of_data_runs(
		     data_attribute,
		     &number_of_data_runs,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d number of data runs.",
			 function,
			 attribute_index );

			goto on_error;
		}
		for( data_run_index = 0;
		     data_run_index < number_of_data_runs;
		     data_run_index++ )
		{
			if( libfsntfs_mft_attribute_get_data_run_by_index(
			     data_attribute,
			     data_run_index,
			     &data_run,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute: %d data run: %d.",
				 function,
				 attribute_index,
				 data_run_index );

				goto on_error;
			}
			if( data_run == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing attribute: %d data run: %d.",
				 function,
				 attribute_index,
				 data_run_index );

				goto on_error;
			}
			if( data_run->size > ( valid_data_size - data_segment_offset ) )
			{
				data_segment_size = valid_data_size - data_segment_offset;
			}
			else
			{
				data_segment_size = data_run->size;
			}
			if( (size64_t) data_segment_offset < valid_data_size )
			{
				if( libfdata_stream_append_segment(
				     safe_data_stream,
				     &segment_index,
				     0,
				     data_run->start_offset,
				     data_segment_size,
				     data_run->range_flags,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append attribute: %d data run: %d data stream segment.",
					 function,
					 attribute_index,
					 data_run_index );

					goto on_error;
				}
				data_segment_offset += data_segment_size;
			}
			calculated_allocated_data_size += data_run->size;
		}
		attribute_index++;

		if( libfsntfs_mft_attribute_get_next_attribute(
		     data_attribute,
		     &data_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next MFT attribute: %d.",
			 function,
			 attribute_index );

			goto on_error;
		}
	}
	if( calculated_allocated_data_size != stored_allocated_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: size of data runs: %" PRIu64 " does not match allocated data size: %" PRIu64 ".",
		 function,
		 calculated_allocated_data_size,
		 stored_allocated_data_size );

		goto on_error;
	}
	if( (size64_t) data_segment_offset < data_size )
	{
		if( libfdata_stream_append_segment(
		     safe_data_stream,
		     &segment_index,
		     0,
		     0,
		     data_size - data_segment_offset,
		     LIBFDATA_RANGE_FLAG_IS_SPARSE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append sparse data stream segment.",
			 function );

			goto on_error;
		}
	}
	*cluster_block_stream = safe_data_stream;

	return( 1 );

on_error:
	if( safe_data_stream != NULL )
	{
		libfdata_stream_free(
		 &safe_data_stream,
		 NULL );
	}
	return( -1 );
}

/* Creates cluster block stream from compressed data runs
 * Make sure the value data_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_stream_initialize_from_compressed_data_runs(
     libfdata_stream_t **cluster_block_stream,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_mft_attribute_t *data_attribute,
     size64_t data_size,
     libcerror_error_t **error )
{
	libfdata_stream_t *safe_data_stream                   = NULL;
	libfsntfs_compressed_block_data_handle_t *data_handle = NULL;
	static char *function                                 = "libfsntfs_cluster_block_stream_initialize_from_compressed_data_runs";
	size64_t valid_data_size                              = 0;
	int segment_index                                     = 0;

	if( cluster_block_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_attribute_get_valid_data_size(
	     data_attribute,
	     &valid_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve valid data size.",
		 function );

		goto on_error;
	}
	if( valid_data_size == 0 )
	{
		valid_data_size = data_size;
	}
	if( libfsntfs_compressed_block_data_handle_initialize(
	     &data_handle,
	     io_handle,
	     data_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     &safe_data_stream,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_compressed_block_data_handle_free,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_compressed_block_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfsntfs_compressed_block_data_handle_seek_segment_offset,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data stream.",
		 function );

		goto on_error;
	}
	data_handle = NULL;

	if( libfdata_stream_append_segment(
	     safe_data_stream,
	     &segment_index,
	     0,
	     0,
	     valid_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append data stream segment.",
		 function );

		goto on_error;
	}
	if( valid_data_size < data_size )
	{
		if( libfdata_stream_append_segment(
		     safe_data_stream,
		     &segment_index,
		     0,
		     0,
		     data_size - valid_data_size,
		     LIBFDATA_RANGE_FLAG_IS_SPARSE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append sparse data stream segment.",
			 function );

			goto on_error;
		}
	}
	*cluster_block_stream = safe_data_stream;

	return( 1 );

on_error:
	if( safe_data_stream != NULL )
	{
		libfdata_stream_free(
		 &safe_data_stream,
		 NULL );
	}
	if( data_handle != NULL )
	{
		libfsntfs_compressed_block_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

/* Creates cluster block stream from a compressed stream
 * Make sure the value data_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_stream_initialize_from_compressed_stream(
     libfdata_stream_t **cluster_block_stream,
     libfdata_stream_t *compressed_data_stream,
     libfsntfs_mft_attribute_t *data_attribute,
     uint32_t compression_method,
     libcerror_error_t **error )
{
	libfdata_stream_t *safe_data_stream             = NULL;
	libfsntfs_compressed_data_handle_t *data_handle = NULL;
	static char *function                           = "libfsntfs_cluster_block_stream_initialize_from_compressed_stream";
	size64_t data_size                              = 0;
	size64_t valid_data_size                        = 0;
	int segment_index                               = 0;

	if( cluster_block_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_attribute_get_data_size(
	     data_attribute,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data size.",
		 function );

		goto on_error;
	}
	if( libfsntfs_mft_attribute_get_valid_data_size(
	     data_attribute,
	     &valid_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve valid data size.",
		 function );

		goto on_error;
	}
	if( valid_data_size == 0 )
	{
		valid_data_size = data_size;
	}
	if( libfsntfs_compressed_data_handle_initialize(
	     &data_handle,
	     compressed_data_stream,
	     compression_method,
	     valid_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     &safe_data_stream,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_compressed_data_handle_free,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_compressed_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfsntfs_compressed_data_handle_seek_segment_offset,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data stream.",
		 function );

		goto on_error;
	}
	data_handle = NULL;

	if( libfdata_stream_append_segment(
	     safe_data_stream,
	     &segment_index,
	     0,
	     0,
	     valid_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append data stream segment.",
		 function );

		goto on_error;
	}
	if( valid_data_size < data_size )
	{
		if( libfdata_stream_append_segment(
		     safe_data_stream,
		     &segment_index,
		     0,
		     0,
		     data_size - valid_data_size,
		     LIBFDATA_RANGE_FLAG_IS_SPARSE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append sparse data stream segment.",
			 function );

			goto on_error;
		}
	}
	*cluster_block_stream = safe_data_stream;

	return( 1 );

on_error:
	if( safe_data_stream != NULL )
	{
		libfdata_stream_free(
		 &safe_data_stream,
		 NULL );
	}
	if( data_handle != NULL )
	{
		libfsntfs_compressed_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

/* Creates a cluster block stream
 * Make sure the value cluster_block_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_stream_initialize(
     libfdata_stream_t **cluster_block_stream,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_mft_attribute_t *data_attribute,
     libfsntfs_mft_attribute_t *wof_compressed_data_attribute,
     uint32_t compression_method,
     libcerror_error_t **error )
{
	libfdata_stream_t *safe_cluster_block_stream = NULL;
	libfsntfs_mft_attribute_t *mft_attribute     = NULL;
	uint8_t *resident_data                       = NULL;
	static char *function                        = "libfsntfs_cluster_block_stream_initialize";
	size64_t data_size                           = 0;
	size_t resident_data_size                    = 0;
	uint16_t data_flags                          = 0;
	int result                                   = 0;

	if( cluster_block_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cluster block stream.",
		 function );

		return( -1 );
	}
	if( wof_compressed_data_attribute == NULL )
	{
		mft_attribute = data_attribute;
	}
	else
	{
		mft_attribute = wof_compressed_data_attribute;
	}
	if( libfsntfs_mft_attribute_get_data_size(
	     mft_attribute,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data size.",
		 function );

		goto on_error;
	}
	result = libfsntfs_mft_attribute_data_is_resident(
	          mft_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if attribute data is resident.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfsntfs_mft_attribute_get_resident_data(
		     mft_attribute,
		     &resident_data,
		     &resident_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve resident data from attribute.",
			 function );

			goto on_error;
		}
	}
	if( libfsntfs_mft_attribute_get_data_flags(
	     mft_attribute,
	     &data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data flags from attribute.",
		 function );

		goto on_error;
	}
	if( ( resident_data != NULL )
	 || ( data_size == 0 ) )
	{
		if( data_size != (size64_t) resident_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: unuspported data size value out of bounds.",
			 function );

			goto on_error;
		}
		result = libfsntfs_cluster_block_stream_initialize_from_data(
		          &safe_cluster_block_stream,
		          resident_data,
		          resident_data_size,
		          error );
	}
	else if( ( data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
	{
		result = libfsntfs_cluster_block_stream_initialize_from_compressed_data_runs(
		          &safe_cluster_block_stream,
		          io_handle,
		          mft_attribute,
		          data_size,
		          error );
	}
	else
	{
		result = libfsntfs_cluster_block_stream_initialize_from_data_runs(
		          &safe_cluster_block_stream,
		          io_handle,
		          mft_attribute,
		          data_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cluster block stream.",
		 function );

		goto on_error;
	}
	if( wof_compressed_data_attribute == NULL )
	{
		*cluster_block_stream = safe_cluster_block_stream;
	}
	else
	{
		if( libfsntfs_cluster_block_stream_initialize_from_compressed_stream(
		     cluster_block_stream,
		     safe_cluster_block_stream,
		     data_attribute,
		     compression_method,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create cluster block stream.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( safe_cluster_block_stream != NULL )
	{
		libfdata_stream_free(
		 &safe_cluster_block_stream,
		 NULL );
	}
	return( -1 );
}

