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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libfsntfs_compressed_block_descriptor.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_unused.h"

/* Creates a compressed block descriptor
 * Make sure the value compressed_block_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_block_descriptor_initialize(
     libfsntfs_compressed_block_descriptor_t **compressed_block_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compressed_block_descriptor_initialize";

	if( compressed_block_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed block descriptor.",
		 function );

		return( -1 );
	}
	if( *compressed_block_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid compressed block descriptor value already set.",
		 function );

		return( -1 );
	}
	*compressed_block_descriptor = memory_allocate_structure(
	                                libfsntfs_compressed_block_descriptor_t );

	if( *compressed_block_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed block descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *compressed_block_descriptor,
	     0,
	     sizeof( libfsntfs_compressed_block_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear compressed block descriptor.",
		 function );

		memory_free(
		 *compressed_block_descriptor );

		*compressed_block_descriptor = NULL;

		return( -1 );
	}
	if( libfdata_stream_initialize(
	     &( ( *compressed_block_descriptor )->data_stream ),
	     NULL,
	     NULL,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_compressed_block_descriptor_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfsntfs_compressed_block_descriptor_seek_segment_offset,
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
	return( 1 );

on_error:
	if( *compressed_block_descriptor != NULL )
	{
		memory_free(
		 *compressed_block_descriptor );

		*compressed_block_descriptor = NULL;
	}
	return( -1 );
}

/* Frees a compressed block descriptor
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_block_descriptor_free(
     libfsntfs_compressed_block_descriptor_t **compressed_block_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compressed_block_descriptor_free";
	int result            = 1;

	if( compressed_block_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed block descriptor.",
		 function );

		return( -1 );
	}
	if( *compressed_block_descriptor != NULL )
	{
		if( libfdata_stream_free(
		     &( ( *compressed_block_descriptor )->data_stream ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data stream.",
			 function );

			result = -1;
		}
		memory_free(
		 *compressed_block_descriptor );

		*compressed_block_descriptor = NULL;
	}
	return( result );
}

/* Appends a data segment
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_block_descriptor_append_data_segment(
     libfsntfs_compressed_block_descriptor_t *compressed_block_descriptor,
     off64_t segment_offset,
     size64_t segment_size,
     uint32_t segment_flags,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compressed_block_descriptor_append_data_segment";
	int segment_index     = 0;

	if( compressed_block_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed block descriptor.",
		 function );

		return( -1 );
	}
	if( libfdata_stream_append_segment(
	     compressed_block_descriptor->data_stream,
	     &segment_index,
	     0,
	     segment_offset,
	     segment_size,
	     segment_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append data run to array.",
		 function );

		return( -1 );
	}
	compressed_block_descriptor->data_size += segment_size;

	return( 1 );
}

/* Reads data from the current offset into a buffer
 * Callback for the compressed block descriptor data stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsntfs_compressed_block_descriptor_read_segment_data(
         intptr_t *data_handle LIBFSNTFS_ATTRIBUTE_UNUSED,
         libbfio_handle_t *file_io_handle,
         int segment_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         int segment_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
         libcerror_error_t **error )
{
	static char *function = "libfsntfs_compressed_block_descriptor_read_segment_data";
	ssize_t read_count    = 0;

	LIBFSNTFS_UNREFERENCED_PARAMETER( data_handle )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_file_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( read_flags )

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
		read_count = libbfio_handle_read_buffer(
		              file_io_handle,
		              segment_data,
		              segment_data_size,
		              error );

		if( read_count != (ssize_t) segment_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read segment data.",
			 function );

			return( -1 );
		}
	}
	return( read_count );
}

/* Seeks a certain offset of the data
 * Callback for the cluster block stream
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfsntfs_compressed_block_descriptor_seek_segment_offset(
         intptr_t *data_handle LIBFSNTFS_ATTRIBUTE_UNUSED,
         libbfio_handle_t *file_io_handle,
         int segment_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         int segment_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         off64_t segment_offset,
         libcerror_error_t **error )
{
	static char *function = "libfsntfs_compressed_block_descriptor_seek_segment_offset";

	LIBFSNTFS_UNREFERENCED_PARAMETER( data_handle )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( segment_file_index )

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
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     segment_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek segment offset: 0x%08" PRIx64 ".",
		 function,
		 segment_offset );

		return( -1 );
	}
	return( segment_offset );
}

#if defined( HAVE_DEBUG_OUTPUT )

/* Debug prints the compressed block descriptor
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compressed_block_descriptor_print(
     libfsntfs_compressed_block_descriptor_t *compressed_block_descriptor,
     libfsntfs_io_handle_t *io_handle,
     int compressed_block_descriptor_index,
     libcerror_error_t **error )
{
	char *compression_unit_data_type = NULL;
	static char *function            = "libfsntfs_compressed_block_descriptor_print";
	off64_t segment_offset           = 0;
	size64_t segment_size            = 0;
	uint32_t segment_flags           = 0;
	int number_of_segments           = 0;
	int segment_file_index           = 0;
	int segment_index                = 0;

	if( compressed_block_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed block descriptor.",
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
	if( ( compressed_block_descriptor->data_range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		compression_unit_data_type = "compressed";
	}
	else if( ( compressed_block_descriptor->data_range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		compression_unit_data_type = "sparse";
	}
	else
	{
		compression_unit_data_type = "uncompressed";
	}
	libcnotify_printf(
	 "%s: %" PRIzd " blocks %s compression unit: %d.\n",
	 function,
	 compressed_block_descriptor->data_size / io_handle->cluster_block_size,
	 compression_unit_data_type,
	 compressed_block_descriptor_index );

	if( libfdata_stream_get_number_of_segments(
	     compressed_block_descriptor->data_stream,
	     &number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments.",
		 function );

		return( -1 );
	}
	for( segment_index = 0;
	     segment_index < number_of_segments;
	     segment_index++ )
	{
		if( libfdata_stream_get_segment_by_index(
		     compressed_block_descriptor->data_stream,
		     segment_index,
		     &segment_file_index,
		     &segment_offset,
		     &segment_size,
		     &segment_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment: %d.",
			 function,
			 segment_index );

			return( -1 );
		}
		if( ( segment_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
		{
			libcnotify_printf(
			 "%s: segment: %d sparse of size: %" PRIu64 "\n",
			 function,
			 segment_index,
			 segment_size );
		}
		else
		{
			libcnotify_printf(
			 "%s: segment: %d at offset: 0x%08" PRIx64 " of size: %" PRIu64 "\n",
			 function,
			 segment_index,
			 segment_offset,
			 segment_size );
		}
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

