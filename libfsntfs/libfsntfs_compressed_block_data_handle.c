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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libfsntfs_compressed_block.h"
#include "libfsntfs_compressed_block_data_handle.h"
#include "libfsntfs_compressed_block_vector.h"
#include "libfsntfs_compression.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_unused.h"

/* Creates compressed block data handle
 * Make sure the value data_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_block_data_handle_initialize(
     libfsntfs_compressed_block_data_handle_t **data_handle,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compressed_block_data_handle_initialize";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data handle value already set.",
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
	*data_handle = memory_allocate_structure(
	                libfsntfs_compressed_block_data_handle_t );

	if( *data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_handle,
	     0,
	     sizeof( libfsntfs_compressed_block_data_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data handle.",
		 function );

		memory_free(
		 *data_handle );

		*data_handle = NULL;

		return( -1 );
	}
	if( libfsntfs_compressed_block_vector_initialize(
	     &( ( *data_handle )->compressed_block_vector ),
	     io_handle,
	     mft_attribute,
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
	if( libfdata_vector_get_size(
	     ( *data_handle )->compressed_block_vector,
	     &( ( *data_handle )->data_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of compressed block vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *data_handle )->compressed_block_cache ),
	     LIBFSNTFS_MAXIMUM_CACHE_ENTRIES_COMPRESSED_BLOCKS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create compressed block cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *data_handle != NULL )
	{
		if( ( *data_handle )->compressed_block_vector != NULL )
		{
			libfdata_vector_free(
			 &( ( *data_handle )->compressed_block_vector ),
			 NULL );
		}
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( -1 );
}

/* Frees a data handle
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_block_data_handle_free(
     libfsntfs_compressed_block_data_handle_t **data_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compressed_block_data_handle_free";
	int result            = 1;

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		if( libfcache_cache_free(
		     &( ( *data_handle )->compressed_block_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free compressed_block cache.",
			 function );

			result = -1;
		}
		if( libfdata_vector_free(
		     &( ( *data_handle )->compressed_block_vector ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free compressed block vector.",
			 function );

			result = -1;
		}
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( result );
}

/* Reads data from the current offset into a compressed block
 * Callback for the data stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsntfs_compressed_block_data_handle_read_segment_data(
         libfsntfs_compressed_block_data_handle_t *data_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         int segment_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
         uint8_t read_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
         libcerror_error_t **error )
{
	libfsntfs_compressed_block_t *compressed_block = NULL;
	static char *function                          = "libfsntfs_compressed_block_data_handle_read_segment_data";
	size_t read_size                               = 0;
	size_t segment_data_offset                     = 0;
	ssize_t read_count                             = 0;
	off64_t compressed_block_offset                = 0;

	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_file_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_flags )
	LIBFSNTFS_UNREFERENCED_PARAMETER( read_flags )

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( data_handle->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data handle - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( segment_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment data.",
		 function );

		return( -1 );
	}
	if( segment_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid segment data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( segment_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		if( memory_set(
		     segment_data,
		     0,
		     segment_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear segment data.",
			 function );

			return( -1 );
		}
		read_count = (ssize_t) segment_data_size;
	}
	else
	{
		if( (size64_t) data_handle->current_offset >= data_handle->data_size )
		{
			return( 0 );
		}
		while( segment_data_size > 0 )
		{
			if( libfdata_vector_get_element_value_at_offset(
			     data_handle->compressed_block_vector,
			     (intptr_t *) file_io_handle,
			     (libfdata_cache_t *) data_handle->compressed_block_cache,
			     data_handle->current_offset,
			     &compressed_block_offset,
			     (intptr_t **) &compressed_block,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve compressed block at offset: %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 data_handle->current_offset,
				 data_handle->current_offset );

				return( -1 );
			}
			if( compressed_block == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid compressed block.",
				 function );

				return( -1 );
			}
			if( compressed_block->data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid compressed block - missing data.",
				 function );

				return( -1 );
			}
			if( ( compressed_block_offset < 0 )
			 || ( (size64_t) compressed_block_offset >= compressed_block->data_size ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid compressed block offset value out of bounds.",
				 function );

				return( -1 );
			}
			read_size = compressed_block->data_size - compressed_block_offset;

			if( read_size > segment_data_size )
			{
				read_size = segment_data_size;
			}
			if( memory_copy(
			     &( segment_data[ segment_data_offset ] ),
			     &( ( compressed_block->data )[ compressed_block_offset ] ),
			     read_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy compressed block data.",
				 function );

				return( -1 );
			}
			segment_data_offset += read_size;
			segment_data_size   -= read_size;

			data_handle->current_offset += read_size;

			if( (size64_t) data_handle->current_offset >= data_handle->data_size )
			{
				break;
			}
		}
		read_count = (ssize_t) segment_data_offset;
	}
	return( read_count );
}

/* Seeks a certain offset of the data
 * Callback for the data stream
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfsntfs_compressed_block_data_handle_seek_segment_offset(
         libfsntfs_compressed_block_data_handle_t *data_handle,
         intptr_t *file_io_handle LIBFSNTFS_ATTRIBUTE_UNUSED,
         int segment_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         int segment_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         off64_t segment_offset,
         libcerror_error_t **error )
{
	static char *function = "libfsntfs_compressed_block_data_handle_seek_segment_offset";

	LIBFSNTFS_UNREFERENCED_PARAMETER( file_io_handle )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_file_index )

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( segment_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment offset value out of bounds.",
		 function );

		return( -1 );
	}
	data_handle->current_offset = segment_offset;

	return( segment_offset );
}

