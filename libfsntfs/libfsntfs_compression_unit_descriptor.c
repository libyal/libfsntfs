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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libfsntfs_cluster_block_data.h"
#include "libfsntfs_compression_unit_descriptor.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfdata.h"

/* Creates a compression unit descriptor
 * Make sure the value compression_unit_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compression_unit_descriptor_initialize(
     libfsntfs_compression_unit_descriptor_t **compression_unit_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compression_unit_descriptor_initialize";

	if( compression_unit_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression unit descriptor.",
		 function );

		return( -1 );
	}
	if( *compression_unit_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid compression unit descriptor value already set.",
		 function );

		return( -1 );
	}
	*compression_unit_descriptor = memory_allocate_structure(
	                                libfsntfs_compression_unit_descriptor_t );

	if( *compression_unit_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compression unit descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *compression_unit_descriptor,
	     0,
	     sizeof( libfsntfs_compression_unit_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear compression unit descriptor.",
		 function );

		memory_free(
		 *compression_unit_descriptor );

		*compression_unit_descriptor = NULL;

		return( -1 );
	}
	if( libfdata_stream_initialize(
	     &( ( *compression_unit_descriptor )->data_stream ),
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
	return( 1 );

on_error:
	if( *compression_unit_descriptor != NULL )
	{
		memory_free(
		 *compression_unit_descriptor );

		*compression_unit_descriptor = NULL;
	}
	return( -1 );
}

/* Frees a compression unit descriptor
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compression_unit_descriptor_free(
     libfsntfs_compression_unit_descriptor_t **compression_unit_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compression_unit_descriptor_free";
	int result            = 1;

	if( compression_unit_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression unit descriptor.",
		 function );

		return( -1 );
	}
	if( *compression_unit_descriptor != NULL )
	{
		if( libfdata_stream_free(
		     &( ( *compression_unit_descriptor )->data_stream ),
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
		 *compression_unit_descriptor );

		*compression_unit_descriptor = NULL;
	}
	return( result );
}

/* Appends a data segment
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compression_unit_descriptor_append_data_segment(
     libfsntfs_compression_unit_descriptor_t *compression_unit_descriptor,
     off64_t segment_offset,
     size64_t segment_size,
     uint32_t segment_flags,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compression_unit_descriptor_append_data_segment";
	int segment_index     = 0;

	if( compression_unit_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression unit descriptor.",
		 function );

		return( -1 );
	}
	if( libfdata_stream_append_segment(
	     compression_unit_descriptor->data_stream,
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
	compression_unit_descriptor->data_size += segment_size;

	return( 1 );
}

#if defined( HAVE_DEBUG_OUTPUT )

/* Debug prints the compression unit descriptor
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compression_unit_descriptor_print(
     libfsntfs_compression_unit_descriptor_t *compression_unit_descriptor,
     libfsntfs_io_handle_t *io_handle,
     int compression_unit_descriptor_index,
     libcerror_error_t **error )
{
	char *compression_unit_data_type = NULL;
	static char *function            = "libfsntfs_compression_unit_descriptor_print";
	size64_t segment_size            = 0;
	off64_t segment_offset           = 0;
	uint32_t segment_flags           = 0;
	int number_of_segments           = 0;
	int segment_file_index           = 0;
	int segment_index                = 0;

	if( compression_unit_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression unit descriptor.",
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
	if( ( compression_unit_descriptor->data_range_flags & LIBFDATA_RANGE_FLAG_IS_COMPRESSED ) != 0 )
	{
		compression_unit_data_type = "compressed";
	}
	else if( ( compression_unit_descriptor->data_range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		compression_unit_data_type = "sparse";
	}
	else
	{
		compression_unit_data_type = "uncompressed";
	}
	libcnotify_printf(
	 "%s: %" PRIu64 " blocks %s compression unit: %d at offset: %" PRIi64 " (0x%08" PRIx64 ").\n",
	 function,
	 compression_unit_descriptor->data_size / io_handle->cluster_block_size,
	 compression_unit_data_type,
	 compression_unit_descriptor_index,
	 compression_unit_descriptor->data_offset,
	 compression_unit_descriptor->data_offset );

	if( libfdata_stream_get_number_of_segments(
	     compression_unit_descriptor->data_stream,
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
		     compression_unit_descriptor->data_stream,
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

