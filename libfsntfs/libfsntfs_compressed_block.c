/*
 * Compressed block functions
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libfsntfs_compressed_block.h"
#include "libfsntfs_compressed_block_descriptor.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_libfwnt.h"
#include "libfsntfs_types.h"
#include "libfsntfs_unused.h"

/* Creates a compressed block
 * Make sure the value compressed_block is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_block_initialize(
     libfsntfs_compressed_block_t **compressed_block,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compressed_block_initialize";

	if( compressed_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed block.",
		 function );

		return( -1 );
	}
	if( *compressed_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid compressed block value already set.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	*compressed_block = memory_allocate_structure(
	                     libfsntfs_compressed_block_t );

	if( *compressed_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *compressed_block,
	     0,
	     sizeof( libfsntfs_compressed_block_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear compressed block.",
		 function );

		memory_free(
		 *compressed_block );

		*compressed_block = NULL;

		return( -1 );
	}
	if( data_size > 0 )
	{
		( *compressed_block )->data = (uint8_t *) memory_allocate(
		                                           sizeof( uint8_t ) * data_size );

		if( ( *compressed_block )->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		( *compressed_block )->data_size = data_size;
	}
	return( 1 );

on_error:
	if( *compressed_block != NULL )
	{
		memory_free(
		 *compressed_block );

		*compressed_block = NULL;
	}
	return( -1 );
}

/* Frees a compressed block
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_block_free(
     libfsntfs_compressed_block_t **compressed_block,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compressed_block_free";

	if( compressed_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed block.",
		 function );

		return( -1 );
	}
	if( *compressed_block != NULL )
	{
		if( ( *compressed_block )->data != NULL )
		{
			memory_free(
			 ( *compressed_block )->data );
		}
		memory_free(
		 *compressed_block );

		*compressed_block = NULL;
	}
	return( 1 );
}

/* Reads a compressed block
 * Callback function for the compressed block vector
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_block_read_element_data(
     libcdata_array_t *compressed_block_descriptors_array,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_data_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
     off64_t element_data_offset LIBFSNTFS_ATTRIBUTE_UNUSED,
     size64_t compressed_block_size,
     uint32_t range_flags,
     uint8_t read_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfsntfs_compressed_block_t *compressed_block                       = NULL;
	libfsntfs_compressed_block_descriptor_t *compressed_block_descriptor = NULL;
	uint8_t *compressed_data                                             = NULL;
	uint8_t *compressed_block_data                                       = NULL;
	static char *function                                                = "libfsntfs_compressed_block_read_element_data";
	ssize_t read_count                                                   = 0;
	int result                                                           = 0;

	LIBFSNTFS_UNREFERENCED_PARAMETER( element_data_file_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( element_data_offset )
	LIBFSNTFS_UNREFERENCED_PARAMETER( read_flags )

	if( compressed_block_descriptors_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed block descriptors array.",
		 function );

		return( -1 );
	}
	if( ( compressed_block_size == 0 )
	 || ( compressed_block_size > (size64_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid compressed block size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     compressed_block_descriptors_array,
	     element_index,
	     (intptr_t **) &compressed_block_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve compressed block descriptor: %d.",
		 function,
		 element_index );

		goto on_error;
	}
/* TODO check compressed_block_descriptor == NULL */
	if( libfsntfs_compressed_block_initialize(
	     &compressed_block,
	     compressed_block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create compressed block.",
		 function );

		goto on_error;
	}
	if( compressed_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing compressed block.",
		 function );

		goto on_error;
	}
	if( ( range_flags & LIBFDATA_RANGE_FLAG_IS_COMPRESSED ) != 0 )
	{
		compressed_data = (uint8_t *) memory_allocate(
		                               sizeof( uint8_t ) * (size_t) compressed_block_size );

		if( compressed_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create compressed data.",
			 function );

			goto on_error;
		}
		compressed_block_data = compressed_data;
	}
	else
	{
		compressed_block_data = compressed_block->data;
	}
	read_count = libfdata_stream_read_buffer(
	              compressed_block_descriptor->data_stream,
		      (intptr_t *) file_io_handle,
		      compressed_block_data,
		      compressed_block_size,
		      0,
		      error );

	if( read_count != (ssize_t) compressed_block_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read compressed block.",
		 function );

		goto on_error;
	}
	if( ( range_flags & LIBFDATA_RANGE_FLAG_IS_COMPRESSED ) != 0 )
	{
		result = libfwnt_lznt1_decompress(
			  compressed_data,
			  (size_t) compressed_block_size,
			  compressed_block->data,
			  &( compressed_block->data_size ),
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
			 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
			 "%s: unable to decompress compressed data.",
			 function );

			goto on_error;
		}
		memory_free(
		 compressed_data );

		compressed_data = NULL;
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     cache,
	     element_index,
	     (intptr_t *) compressed_block,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_compressed_block_free,
	     LIBFDATA_VECTOR_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set cluster block as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( compressed_data != NULL )
	{
		memory_free(
		 compressed_data );
	}
	if( compressed_block != NULL )
	{
		libfsntfs_compressed_block_free(
		 &compressed_block,
		 NULL );
	}
	return( -1 );
}

