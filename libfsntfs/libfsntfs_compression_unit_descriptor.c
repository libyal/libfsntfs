/*
 * Compression unit descriptor functions
 *
 * Copyright (C) 2010-2015, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsntfs_compression_unit_descriptor.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_unused.h"

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
	     NULL, /* read */
	     NULL,
	     NULL, /* seek */
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

/* Reads data from the current offset into a buffer
 * Callback for the compression unit descriptor data stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsntfs_compression_unit_descriptor_read_segment_data(
         intptr_t *data_handle LIBFSNTFS_ATTRIBUTE_UNUSED,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         int segment_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
         uint8_t read_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
         libcerror_error_t **error )
{
	static char *function      = "libfsntfs_compression_unit_descriptor_read_segment_data";
	size_t segment_data_offset = 0;

	LIBFSNTFS_UNREFERENCED_PARAMETER( data_handle )
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
	while( segment_data_size > 0 )
	{
/* TODO */
	}
	return( (ssize_t) segment_data_offset );
}

