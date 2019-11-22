/*
 * Cluster block stream functions
 *
 * Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libfsntfs_buffer_data_handle.h"
#include "libfsntfs_cluster_block_data_handle.h"
#include "libfsntfs_cluster_block_stream.h"
#include "libfsntfs_compressed_block_data_handle.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_unused.h"

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
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error )
{
	libfdata_stream_t *safe_data_stream                = NULL;
	libfsntfs_cluster_block_data_handle_t *data_handle = NULL;
	static char *function                              = "libfsntfs_cluster_block_stream_initialize_from_data_runs";
	size64_t valid_data_size                           = 0;
	int segment_index                                  = 0;

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
	     mft_attribute,
	     &valid_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve valid data size",
		 function );

		goto on_error;
	}
	if( valid_data_size == 0 )
	{
		if( libfsntfs_mft_attribute_get_data_size(
		     mft_attribute,
		     &valid_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data size",
			 function );

			goto on_error;
		}
	}
	if( libfsntfs_cluster_block_data_handle_initialize(
	     &data_handle,
	     io_handle,
	     mft_attribute,
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
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_cluster_block_data_handle_free,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_cluster_block_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfsntfs_cluster_block_data_handle_seek_segment_offset,
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
		libfsntfs_cluster_block_data_handle_free(
		 &data_handle,
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
     libfsntfs_mft_attribute_t *mft_attribute,
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
	     mft_attribute,
	     &valid_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve valid data size",
		 function );

		goto on_error;
	}
	if( valid_data_size == 0 )
	{
		if( libfsntfs_mft_attribute_get_data_size(
		     mft_attribute,
		     &valid_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data size",
			 function );

			goto on_error;
		}
	}
	if( libfsntfs_compressed_block_data_handle_initialize(
	     &data_handle,
	     io_handle,
	     mft_attribute,
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

/* Creates a cluster block stream
 * Make sure the value cluster_block_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_cluster_block_stream_initialize(
     libfdata_stream_t **cluster_block_stream,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error )
{
	libfdata_stream_t *safe_cluster_block_stream = NULL;
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
	if( libfsntfs_mft_attribute_get_data(
	     mft_attribute,
	     &resident_data,
	     &resident_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve resident attribute data",
		 function );

		goto on_error;
	}
	if( libfsntfs_mft_attribute_get_valid_data_size(
	     mft_attribute,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve valid data size",
		 function );

		goto on_error;
	}
	if( data_size == 0 )
	{
		if( libfsntfs_mft_attribute_get_data_size(
		     mft_attribute,
		     &data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data size",
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
		 "%s: unable to retrieve attribute data flags.",
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
	else if( ( data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) == 0 )
	{
		result = libfsntfs_cluster_block_stream_initialize_from_data_runs(
		          &safe_cluster_block_stream,
		          io_handle,
		          mft_attribute,
		          error );
	}
	else
	{
		result = libfsntfs_cluster_block_stream_initialize_from_compressed_data_runs(
		          &safe_cluster_block_stream,
		          io_handle,
		          mft_attribute,
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
	*cluster_block_stream = safe_cluster_block_stream;

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

