/*
 * The compressed data handle functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsntfs_compressed_data_handle.h"
#include "libfsntfs_compression.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_unused.h"

/* Creates compressed data handle
 * Make sure the value data_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_data_handle_initialize(
     libfsntfs_compressed_data_handle_t **data_handle,
     libfdata_stream_t *compressed_data_stream,
     uint32_t compression_method,
     size64_t uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function        = "libfsntfs_compressed_data_handle_initialize";
	size_t compression_unit_size = 0;

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
	if( compressed_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data stream.",
		 function );

		return( -1 );
	}
	switch( compression_method )
	{
		case 0:
			compression_method    = LIBFSNTFS_COMPRESSION_METHOD_LZXPRESS_HUFFMAN;
			compression_unit_size = 4096;
			break;

		case 1:
			compression_method    = LIBFSNTFS_COMPRESSION_METHOD_LZX;
			compression_unit_size = 32768;
			break;

		case 2:
			compression_method    = LIBFSNTFS_COMPRESSION_METHOD_LZXPRESS_HUFFMAN;
			compression_unit_size = 8192;
			break;

		case 3:
			compression_method    = LIBFSNTFS_COMPRESSION_METHOD_LZXPRESS_HUFFMAN;
			compression_unit_size = 16384;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported compression method: %" PRIu32 ".",
			 function,
			 compression_method );

			return( -1 );
	}
	*data_handle = memory_allocate_structure(
	                libfsntfs_compressed_data_handle_t );

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
	     sizeof( libfsntfs_compressed_data_handle_t ) ) == NULL )
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
	( *data_handle )->compressed_segment_data = (uint8_t *) memory_allocate(
	                                                         sizeof( uint8_t ) * compression_unit_size );

	if( ( *data_handle )->compressed_segment_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed segment data.",
		 function );

		goto on_error;
	}
	( *data_handle )->segment_data = (uint8_t *) memory_allocate(
	                                              sizeof( uint8_t ) * compression_unit_size );

	if( ( *data_handle )->segment_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment data.",
		 function );

		goto on_error;
	}
	( *data_handle )->compressed_data_stream         = compressed_data_stream;
	( *data_handle )->current_compressed_block_index = (uint32_t) -1;
	( *data_handle )->uncompressed_data_size         = uncompressed_data_size;
	( *data_handle )->compression_method             = (int) compression_method;
	( *data_handle )->compression_unit_size          = compression_unit_size;

	return( 1 );

on_error:
	if( *data_handle != NULL )
	{
		if( ( *data_handle )->segment_data != NULL )
		{
			memory_free(
			 ( *data_handle )->segment_data );
		}
		if( ( *data_handle )->compressed_segment_data != NULL )
		{
			memory_free(
			 ( *data_handle )->compressed_segment_data );
		}
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( -1 );
}

/* Frees data handle
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_data_handle_free(
     libfsntfs_compressed_data_handle_t **data_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compressed_data_handle_free";

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
		if( ( *data_handle )->segment_data != NULL )
		{
			memory_free(
			 ( *data_handle )->segment_data );
		}
		if( ( *data_handle )->compressed_segment_data != NULL )
		{
			memory_free(
			 ( *data_handle )->compressed_segment_data );
		}
		if( ( *data_handle )->compressed_block_offsets != NULL )
		{
			memory_free(
			 ( *data_handle )->compressed_block_offsets );
		}
		if( ( *data_handle )->compressed_data_stream != NULL )
		{
			libfdata_stream_free(
			 &( ( *data_handle )->compressed_data_stream ),
			 NULL );
		}
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( 1 );
}

/* Determines the compressed block offsets
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_data_handle_get_compressed_block_offsets(
     libfsntfs_compressed_data_handle_t *data_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	uint8_t *chunk_offsets_data               = NULL;
	static char *function                     = "libfsntfs_compressed_data_handle_get_compressed_block_offsets";
	size64_t compressed_data_size             = 0;
	size_t chunk_offset_data_size             = 0;
	size_t chunk_offsets_data_size            = 0;
	size_t data_offset                        = 0;
	ssize_t read_count                        = 0;
	uint64_t compressed_block_index           = 0;
	uint64_t compressed_block_offset          = 0;
	uint64_t previous_compressed_block_offset = 0;

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
	if( data_handle->compressed_block_offsets != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data handle - compressed block offsets value already set.",
		 function );

		return( -1 );
	}
	if( libfdata_stream_get_size(
	     data_handle->compressed_data_stream,
	     &compressed_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve compressed data size.",
		 function );

		goto on_error;
	}
	if( compressed_data_size > (size64_t) UINT32_MAX )
	{
		chunk_offset_data_size = 8;
	}
	else
	{
		chunk_offset_data_size = 4;
	}
	chunk_offsets_data_size = data_handle->uncompressed_data_size / data_handle->compression_unit_size;

	if( ( data_handle->uncompressed_data_size % data_handle->compression_unit_size ) != 0 )
	{
		chunk_offsets_data_size++;
	}
	if( chunk_offsets_data_size > ( (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE / chunk_offset_data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of chunk offsets data size value exceeds maximum allocation size.",
		 function );

		goto on_error;
	}
	chunk_offsets_data_size *= chunk_offset_data_size;

	chunk_offsets_data = (uint8_t *) memory_allocate(
	                                  sizeof( uint8_t ) * chunk_offsets_data_size );

	if( chunk_offsets_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create chunk offsets data.",
		 function );

		goto on_error;
	}
	read_count = libfdata_stream_read_buffer_at_offset(
	              data_handle->compressed_data_stream,
	              (intptr_t *) file_io_handle,
	              chunk_offsets_data,
	              chunk_offsets_data_size,
	              0,
	              0,
	              error );

	if( read_count != (ssize_t) chunk_offsets_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read chunk offsets data from compressed data stream.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: chunk offsets data:\n",
			 function );
			libcnotify_print_data(
			 chunk_offsets_data,
			 chunk_offsets_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
	previous_compressed_block_offset = chunk_offset_data_size;

	compressed_block_index++;

	for( data_offset = 0;
	     data_offset < chunk_offsets_data_size;
	     data_offset += chunk_offset_data_size )
	{
		if( chunk_offset_data_size == 8 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 &( chunk_offsets_data[ data_offset ] ),
			 compressed_block_offset );
		}
		else
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( chunk_offsets_data[ data_offset ] ),
			 compressed_block_offset );
		}
		if( ( compressed_block_offset <= previous_compressed_block_offset )
		 || ( compressed_block_offset >= compressed_data_size ) )
		{
			break;
		}
		previous_compressed_block_offset = compressed_block_offset;

		compressed_block_index++;
	}
	data_handle->number_of_compressed_blocks = compressed_block_index;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of compressed blocks\t: %" PRIu64 "\n",
		 function,
		 data_handle->number_of_compressed_blocks );
	}
#endif
	if( (size_t) data_handle->number_of_compressed_blocks > ( (size_t) ( MEMORY_MAXIMUM_ALLOCATION_SIZE / sizeof( uint64_t ) ) - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of compressed blocks value exceeds maximum allocation size.",
		 function );

		goto on_error;
	}
	data_handle->compressed_block_offsets = (uint64_t *) memory_allocate(
	                                                      sizeof( uint64_t ) * ( data_handle->number_of_compressed_blocks + 1 ) );

	if( data_handle->compressed_block_offsets == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed block offsets.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: compressed block: % 2d offset\t: %" PRIzd " (0x%08" PRIzx ")\n",
		 function,
		 0,
		 data_offset,
		 data_offset );
	}
#endif
	data_handle->compressed_block_offsets[ 0 ] = data_offset;

	data_offset = 0;

	for( compressed_block_index = 1;
	     compressed_block_index < data_handle->number_of_compressed_blocks;
	     compressed_block_index++ )
	{
		if( chunk_offset_data_size == 8 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 &( chunk_offsets_data[ data_offset ] ),
			 compressed_block_offset );
		}
		else
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( chunk_offsets_data[ data_offset ] ),
			 compressed_block_offset );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: compressed block: % 2" PRIu64 " offset\t: %" PRIu64 " (0x%08" PRIx64 ")\n",
			 function,
			 compressed_block_index,
			 compressed_block_offset,
			 compressed_block_offset );
		}
#endif
		if( ( compressed_block_offset == 0 )
		 || ( compressed_block_offset >= ( (uint64_t) INT64_MAX - data_handle->compressed_block_offsets[ 0 ] ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid compressed block offset: %d value out of bounds.",
			 function,
			 compressed_block_index );

			goto on_error;
		}
		data_offset += chunk_offset_data_size;

		data_handle->compressed_block_offsets[ compressed_block_index ] = data_handle->compressed_block_offsets[ 0 ] + compressed_block_offset;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: compressed block: % 2" PRIu64 " offset\t: %" PRIu64 " (0x%08" PRIx64 ")\n",
		 function,
		 compressed_block_index,
		 compressed_data_size,
		 compressed_data_size );
	}
#endif
	data_handle->compressed_block_offsets[ compressed_block_index ] = (uint64_t) compressed_data_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	memory_free(
	 chunk_offsets_data );

	return( 1 );

on_error:
	if( data_handle->compressed_block_offsets != NULL )
	{
		memory_free(
		 data_handle->compressed_block_offsets );

		data_handle->compressed_block_offsets = NULL;
	}
	if( chunk_offsets_data != NULL )
	{
		memory_free(
		 chunk_offsets_data );
	}
	return( -1 );
}

/* Reads data from the current offset into a compressed
 * Callback for the data stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsntfs_compressed_data_handle_read_segment_data(
         libfsntfs_compressed_data_handle_t *data_handle,
         intptr_t *file_io_handle,
         int segment_index,
         int segment_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
         uint8_t read_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
         libcerror_error_t **error )
{
	uint8_t *read_buffer              = NULL;
	static char *function             = "libfsntfs_compressed_data_handle_read_segment_data";
	size_t data_offset                = 0;
	size_t read_size                  = 0;
	size_t segment_data_offset        = 0;
	ssize_t read_count                = 0;
	off64_t data_stream_offset        = 0;
	off64_t uncompressed_block_offset = 0;
	uint64_t compressed_block_index   = 0;

	LIBFSNTFS_UNREFERENCED_PARAMETER( file_io_handle )
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
	if( segment_index != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment index value out of bounds.",
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
	if( data_handle->compressed_block_offsets == NULL )
	{
		if( libfsntfs_compressed_data_handle_get_compressed_block_offsets(
		     data_handle,
		     (libbfio_handle_t *) file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine compressed block offsets.",
			 function );

			return( -1 );
		}
	}
	if( (size64_t) data_handle->current_offset >= data_handle->uncompressed_data_size )
	{
		return( 0 );
	}
	compressed_block_index = (uint64_t) ( data_handle->current_offset / data_handle->compression_unit_size );
	data_offset            = (size_t) ( data_handle->current_offset % data_handle->compression_unit_size );

	while( segment_data_size > 0 )
	{
		if( compressed_block_index >= data_handle->number_of_compressed_blocks )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid compressed block index value out of bounds.",
			 function );

			return( -1 );
		}
		if( data_handle->current_compressed_block_index != compressed_block_index )
		{
			data_stream_offset = data_handle->compressed_block_offsets[ compressed_block_index ];
			read_size          = (size_t) ( data_handle->compressed_block_offsets[ compressed_block_index + 1 ] - data_stream_offset );

			data_handle->segment_data_size = data_handle->compression_unit_size;

			if( data_handle->segment_data_size > ( data_handle->uncompressed_data_size - data_handle->current_offset ) )
			{
				data_handle->segment_data_size = data_handle->uncompressed_data_size - data_handle->current_offset;
			}
			if( read_size > data_handle->segment_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid segment data size value out of bounds.",
				 function );

				return( -1 );
			}
			if( read_size < data_handle->segment_data_size )
			{
				read_buffer = data_handle->compressed_segment_data;
			}
			else
			{
				read_buffer = data_handle->segment_data;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: compressed block: % 2" PRIu64 " offset\t: %" PRIu64 " (0x%08" PRIx64 ")\n",
				 function,
				 compressed_block_index,
				 data_stream_offset,
				 data_stream_offset );

				libcnotify_printf(
				 "\n" );
			}
#endif
			read_count = libfdata_stream_read_buffer_at_offset(
			              data_handle->compressed_data_stream,
			              (intptr_t *) file_io_handle,
			              read_buffer,
			              read_size,
			              data_stream_offset,
			              0,
			              error );

			if( read_count != (ssize_t) read_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read buffer at offset: %" PRIi64 " (0x%08" PRIx64 ") from data stream.",
				 function,
				 data_stream_offset,
				 data_stream_offset );

				return( -1 );
			}
			if( read_buffer == data_handle->compressed_segment_data )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: compressed block data:\n",
					 function );
					libcnotify_print_data(
					 data_handle->compressed_segment_data,
					 read_size,
					 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
				}
#endif
				if( libfsntfs_decompress_data(
				     data_handle->compressed_segment_data,
				     (size_t) read_count,
				     data_handle->compression_method,
				     data_handle->segment_data,
				     &( data_handle->segment_data_size ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
					 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
					 "%s: unable to decompress data.",
					 function );

					return( -1 );
				}
				uncompressed_block_offset = ( compressed_block_index + 1 ) * data_handle->compression_unit_size;

				if( ( (size64_t) uncompressed_block_offset < data_handle->uncompressed_data_size )
				 && ( data_handle->segment_data_size != data_handle->compression_unit_size ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid uncompressed segment data size value out of bounds.",
					 function );

					return( -1 );
				}
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: uncompressed block data:\n",
				 function );
				libcnotify_print_data(
				 data_handle->segment_data,
				 data_handle->segment_data_size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
#endif
			data_handle->current_compressed_block_index = compressed_block_index;
		}
		if( data_offset >= data_handle->segment_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data offset value out of bounds.",
			 function );

			return( -1 );
		}
		read_size = data_handle->segment_data_size - data_offset;

		if( read_size > segment_data_size )
		{
			read_size = segment_data_size;
		}
		if( memory_copy(
		     &( segment_data[ segment_data_offset ] ),
		     &( data_handle->segment_data[ data_offset ] ),
		     read_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy data.",
			 function );

			return( -1 );
		}
		data_offset          = 0;
		segment_data_size   -= read_size;
		segment_data_offset += read_size;

		compressed_block_index++;

		data_handle->current_offset += read_size;

		if( (size64_t) data_handle->current_offset >= data_handle->uncompressed_data_size )
		{
			break;
		}
	}
	return( (ssize_t) segment_data_offset );
}

/* Seeks a certain offset of the data
 * Callback for the data stream
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfsntfs_compressed_data_handle_seek_segment_offset(
         libfsntfs_compressed_data_handle_t *data_handle,
         intptr_t *file_io_handle LIBFSNTFS_ATTRIBUTE_UNUSED,
         int segment_index,
         int segment_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         off64_t segment_offset,
         libcerror_error_t **error )
{
	static char *function = "libfsntfs_compressed_data_handle_seek_segment_offset";

	LIBFSNTFS_UNREFERENCED_PARAMETER( file_io_handle )
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
	if( segment_index != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment index value out of bounds.",
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

