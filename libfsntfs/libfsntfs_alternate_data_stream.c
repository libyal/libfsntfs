/*
 * Alternate data stream functions
 *
 * Copyright (c) 2010-2014, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsntfs_alternate_data_stream.h"
#include "libfsntfs_attribute.h"
#include "libfsntfs_cluster_block_stream.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_types.h"

/* Creates an alternate data stream
 * Make sure the value alternate_data_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_alternate_data_stream_initialize(
     libfsntfs_alternate_data_stream_t **alternate_data_stream,
     libbfio_handle_t *file_io_handle,
     libfsntfs_io_handle_t *io_handle,
     libfsntfs_attribute_t *data_attribute,
     libcerror_error_t **error )
{
	libfsntfs_internal_alternate_data_stream_t *internal_alternate_data_stream = NULL;
	static char *function                                                      = "libfsntfs_alternate_data_stream_initialize";

	if( alternate_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alternate data stream.",
		 function );

		return( -1 );
	}
	if( data_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data attribute.",
		 function );

		return( -1 );
	}
	if( *alternate_data_stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid alternate data stream value already set.",
		 function );

		return( -1 );
	}
	internal_alternate_data_stream = memory_allocate_structure(
	                                  libfsntfs_internal_alternate_data_stream_t );

	if( internal_alternate_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create alternate data stream.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     internal_alternate_data_stream,
	     0,
	     sizeof( libfsntfs_internal_alternate_data_stream_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear alternate data stream.",
		 function );

		memory_free(
		 internal_alternate_data_stream );

		return( -1 );
	}
	if( libfsntfs_cluster_block_stream_initialize(
	     &( internal_alternate_data_stream->data_cluster_block_stream ),
	     io_handle,
	     data_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data cluster block stream.",
		 function );

		goto on_error;
	}
	internal_alternate_data_stream->file_io_handle = file_io_handle;
	internal_alternate_data_stream->data_attribute = data_attribute;

	*alternate_data_stream = (libfsntfs_alternate_data_stream_t *) internal_alternate_data_stream;

	return( 1 );

on_error:
	if( internal_alternate_data_stream != NULL )
	{
		memory_free(
		 internal_alternate_data_stream );
	}
	return( -1 );
}

/* Frees an alternate data stream
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_alternate_data_stream_free(
     libfsntfs_alternate_data_stream_t **alternate_data_stream,
     libcerror_error_t **error )
{
	libfsntfs_internal_alternate_data_stream_t *internal_alternate_data_stream = NULL;
	static char *function                                                      = "libfsntfs_alternate_data_stream_free";
	int result                                                                 = 1;

	if( alternate_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alternate data stream.",
		 function );

		return( -1 );
	}
	if( *alternate_data_stream != NULL )
	{
		internal_alternate_data_stream = (libfsntfs_internal_alternate_data_stream_t *) *alternate_data_stream;
		*alternate_data_stream         = NULL;

		/* The file_io_handle and data_attribute references are freed elsewhere
		 */
		if( libfdata_stream_free(
		     &( internal_alternate_data_stream->data_cluster_block_stream ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data cluster block stream.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_alternate_data_stream );
	}
	return( result );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_alternate_data_stream_get_utf8_name_size(
     libfsntfs_alternate_data_stream_t *alternate_data_stream,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_alternate_data_stream_t *internal_alternate_data_stream = NULL;
	static char *function                                                      = "libfsntfs_alternate_data_stream_get_utf8_name_size";

	if( alternate_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alternate data stream.",
		 function );

		return( -1 );
	}
	internal_alternate_data_stream = (libfsntfs_internal_alternate_data_stream_t *) alternate_data_stream;

	if( internal_alternate_data_stream->data_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alternate data stream - missing data attribute.",
		 function );

		return( -1 );
	}
	if( libfsntfs_attribute_get_utf8_name_size(
	     internal_alternate_data_stream->data_attribute,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 name from data stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_alternate_data_stream_get_utf8_name(
     libfsntfs_alternate_data_stream_t *alternate_data_stream,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_alternate_data_stream_t *internal_alternate_data_stream = NULL;
	static char *function                                                      = "libfsntfs_alternate_data_stream_get_utf8_name";

	if( alternate_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alternate data stream.",
		 function );

		return( -1 );
	}
	internal_alternate_data_stream = (libfsntfs_internal_alternate_data_stream_t *) alternate_data_stream;

	if( internal_alternate_data_stream->data_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alternate data stream - missing data attribute.",
		 function );

		return( -1 );
	}
	if( libfsntfs_attribute_get_utf8_name(
	     internal_alternate_data_stream->data_attribute,
	     utf8_name,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name from data stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_alternate_data_stream_get_utf16_name_size(
     libfsntfs_alternate_data_stream_t *alternate_data_stream,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_alternate_data_stream_t *internal_alternate_data_stream = NULL;
	static char *function                                                      = "libfsntfs_alternate_data_stream_get_utf16_name_size";

	if( alternate_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alternate data stream.",
		 function );

		return( -1 );
	}
	internal_alternate_data_stream = (libfsntfs_internal_alternate_data_stream_t *) alternate_data_stream;

	if( internal_alternate_data_stream->data_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alternate data stream - missing data attribute.",
		 function );

		return( -1 );
	}
	if( libfsntfs_attribute_get_utf16_name_size(
	     internal_alternate_data_stream->data_attribute,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 name from data stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_alternate_data_stream_get_utf16_name(
     libfsntfs_alternate_data_stream_t *alternate_data_stream,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_alternate_data_stream_t *internal_alternate_data_stream = NULL;
	static char *function                                                      = "libfsntfs_alternate_data_stream_get_utf16_name";

	if( alternate_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alternate data stream.",
		 function );

		return( -1 );
	}
	internal_alternate_data_stream = (libfsntfs_internal_alternate_data_stream_t *) alternate_data_stream;

	if( internal_alternate_data_stream->data_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid alternate data stream - missing data stream.",
		 function );

		return( -1 );
	}
	if( libfsntfs_attribute_get_utf16_name(
	     internal_alternate_data_stream->data_attribute,
	     utf16_name,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name from data stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads data at the current offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsntfs_alternate_data_stream_read_buffer(
         libfsntfs_alternate_data_stream_t *alternate_data_stream,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libfsntfs_internal_alternate_data_stream_t *internal_alternate_data_stream = NULL;
	static char *function                                                      = "libfsntfs_alternate_data_stream_read_buffer";
	ssize_t read_count                                                         = 0;

	if( alternate_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alternate data stream.",
		 function );

		return( -1 );
	}
	internal_alternate_data_stream = (libfsntfs_internal_alternate_data_stream_t *) alternate_data_stream;

	read_count = libfdata_stream_read_buffer(
	              internal_alternate_data_stream->data_cluster_block_stream,
		      (intptr_t *) internal_alternate_data_stream->file_io_handle,
		      buffer,
		      buffer_size,
		      0,
		      error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from data cluster block stream.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsntfs_alternate_data_stream_read_buffer_at_offset(
         libfsntfs_alternate_data_stream_t *alternate_data_stream,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libfsntfs_alternate_data_stream_read_buffer_at_offset";
	ssize_t read_count    = 0;

	if( libfsntfs_alternate_data_stream_seek_offset(
	     alternate_data_stream,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	read_count = libfsntfs_alternate_data_stream_read_buffer(
	              alternate_data_stream,
	              buffer,
	              buffer_size,
	              error );

	if( read_count <= -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Seeks a certain offset
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfsntfs_alternate_data_stream_seek_offset(
         libfsntfs_alternate_data_stream_t *alternate_data_stream,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libfsntfs_internal_alternate_data_stream_t *internal_alternate_data_stream = NULL;
	static char *function                                                      = "libfsntfs_alternate_data_stream_seek_offset";

	if( alternate_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alternate data stream.",
		 function );

		return( -1 );
	}
	internal_alternate_data_stream = (libfsntfs_internal_alternate_data_stream_t *) alternate_data_stream;

	offset = libfdata_stream_seek_offset(
	          internal_alternate_data_stream->data_cluster_block_stream,
		  offset,
		  whence,
		  error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in data cluster block stream.",
		 function );

		return( -1 );
	}
	return( offset );
}

/* Retrieves the current offset
 * Returns the offset if successful or -1 on error
 */
int libfsntfs_alternate_data_stream_get_offset(
     libfsntfs_alternate_data_stream_t *alternate_data_stream,
     off64_t *offset,
     libcerror_error_t **error )
{
	libfsntfs_internal_alternate_data_stream_t *internal_alternate_data_stream = NULL;
	static char *function                                                      = "libfsntfs_alternate_data_stream_get_offset";

	if( alternate_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alternate data stream.",
		 function );

		return( -1 );
	}
	internal_alternate_data_stream = (libfsntfs_internal_alternate_data_stream_t *) alternate_data_stream;

	if( libfdata_stream_get_offset(
	     internal_alternate_data_stream->data_cluster_block_stream,
	     offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve offset from data cluster block stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_alternate_data_stream_get_size(
     libfsntfs_alternate_data_stream_t *alternate_data_stream,
     size64_t *size,
     libcerror_error_t **error )
{
	libfsntfs_internal_alternate_data_stream_t *internal_alternate_data_stream = NULL;
	static char *function                                                      = "libfsntfs_alternate_data_stream_get_size";

	if( alternate_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid alternate data stream.",
		 function );

		return( -1 );
	}
	internal_alternate_data_stream = (libfsntfs_internal_alternate_data_stream_t *) alternate_data_stream;

	if( libfdata_stream_get_size(
	     internal_alternate_data_stream->data_cluster_block_stream,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data cluster block stream size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

