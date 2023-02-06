/*
 * Compressed block vector functions
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
#include <memory.h>
#include <types.h>

#include "libfsntfs_compressed_block.h"
#include "libfsntfs_compressed_block_vector.h"
#include "libfsntfs_compression_unit_data_handle.h"
#include "libfsntfs_compression_unit_descriptor.h"
#include "libfsntfs_compression.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_unused.h"

/* Creates a compressed block vector
 * Make sure the value compressed_block_vector is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_block_vector_initialize(
     libfdata_vector_t **compressed_block_vector,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error )
{
	libfdata_vector_t *safe_compressed_block_vector       = NULL;
	libfsntfs_compression_unit_data_handle_t *data_handle = NULL;
	libfsntfs_compression_unit_descriptor_t *descriptor   = NULL;
	static char *function                                 = "libfsntfs_compressed_block_vector_initialize";
	int descriptor_index                                  = 0;
	int number_of_descriptors                             = 0;
	int segment_index                                     = 0;

	if( compressed_block_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed block vector.",
		 function );

		return( -1 );
	}
	if( *compressed_block_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid compressed block vector value already set.",
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
	if( libfsntfs_compression_unit_data_handle_initialize(
	     &data_handle,
	     io_handle,
	     mft_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create compression unit data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_initialize(
	     &safe_compressed_block_vector,
	     (size64_t) data_handle->compression_unit_size,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_compression_unit_data_handle_free,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_compressed_block_vector_read_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create compressed block vector.",
		 function );

		goto on_error;
	}
	if( libfsntfs_compression_unit_data_handle_get_number_of_descriptors(
	     data_handle,
	     &number_of_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of descriptors.",
		 function );

		data_handle = NULL;

		goto on_error;
	}
	for( descriptor_index = 0;
	     descriptor_index < number_of_descriptors;
	     descriptor_index++ )
	{
		if( libfsntfs_compression_unit_data_handle_get_descriptor_by_index(
		     data_handle,
		     descriptor_index,
		     &descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve descriptor: %d.",
			 function,
			 descriptor_index );

			data_handle = NULL;

			goto on_error;
		}
		if( descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing descriptor: %d.",
			 function,
			 descriptor_index );

			data_handle = NULL;

			goto on_error;
		}
		if( libfdata_vector_append_segment(
		     safe_compressed_block_vector,
		     &segment_index,
		     descriptor_index,
		     descriptor->data_offset,
		     descriptor->compression_unit_size,
		     descriptor->data_range_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append compression unit: %d segment to compressed block vector.",
			 function,
			 descriptor_index );

			data_handle = NULL;

			goto on_error;
		}
	}
	*compressed_block_vector = safe_compressed_block_vector;

	return( 1 );

on_error:
	if( safe_compressed_block_vector != NULL )
	{
		libfdata_vector_free(
		 &safe_compressed_block_vector,
		 NULL );
	}
	if( data_handle != NULL )
	{
		libfsntfs_compression_unit_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

/* Reads a compressed block
 * Callback function for the compressed block vector
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_block_vector_read_element_data(
     libfsntfs_compression_unit_data_handle_t *data_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfdata_cache_t *cache,
     int element_index,
     int element_data_file_index,
     off64_t element_data_offset,
     size64_t compressed_block_size,
     uint32_t range_flags,
     uint8_t read_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfsntfs_compressed_block_t *compressed_block                       = NULL;
	libfsntfs_compression_unit_descriptor_t *compression_unit_descriptor = NULL;
	uint8_t *compressed_block_data                                       = NULL;
	uint8_t *compressed_data                                             = NULL;
	const char *block_type                                               = NULL;
	static char *function                                                = "libfsntfs_compressed_block_vector_read_element_data";
	ssize_t read_count                                                   = 0;
	off64_t data_stream_offset                                           = 0;
	int result                                                           = 0;

	LIBFSNTFS_UNREFERENCED_PARAMETER( read_flags )

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
	if( libfsntfs_compression_unit_data_handle_get_descriptor_by_index(
	     data_handle,
	     element_data_file_index,
	     &compression_unit_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve compression unit descriptor: %d.",
		 function,
		 element_data_file_index );

		goto on_error;
	}
	if( compression_unit_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing compression unit descriptor: %d.",
		 function,
		 element_data_file_index );

		goto on_error;
	}
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

		/* Make sure to read from the start of the data stream
		 * otherwise successive reads will fail
		 */
		data_stream_offset = 0;

		block_type = "compressed";
	}
	else
	{
		compressed_block_data = compressed_block->data;
		data_stream_offset    = element_data_offset - compression_unit_descriptor->data_offset;

		block_type = "uncompressed";
	}
	read_count = libfdata_stream_read_buffer_at_offset(
	              compression_unit_descriptor->data_stream,
	              (intptr_t *) file_io_handle,
	              compressed_block_data,
	              compressed_block_size,
	              data_stream_offset,
	              0,
	              error );

	if( read_count != (ssize_t) compressed_block_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read %s block at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 block_type,
		 data_stream_offset,
		 data_stream_offset );

		goto on_error;
	}
	if( ( range_flags & LIBFDATA_RANGE_FLAG_IS_COMPRESSED ) != 0 )
	{
		result = libfsntfs_decompress_data(
		          compressed_data,
		          (size_t) compressed_block_size,
		          LIBFSNTFS_COMPRESSION_METHOD_LZNT1,
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

		/* If the compressed block data size is 0 or the compressed block was truncated
		 * fill the remainder of the compressed block with 0-byte values
		 */
		if( compressed_block->data_size < compressed_block_size )
		{
			if( memory_set(
			     &( compressed_block->data[ compressed_block->data_size ] ),
			     0,
			     compressed_block_size - compressed_block->data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear remainder of compressed block.",
				 function );

				goto on_error;
			}
			compressed_block->data_size = compressed_block_size;
		}
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

