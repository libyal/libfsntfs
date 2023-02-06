/*
 * Data stream functions
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

#include "libfsntfs_cluster_block_stream.h"
#include "libfsntfs_data_stream.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_extent.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcthreads.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_types.h"

/* Creates a data stream
 * Make sure the value data_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_stream_initialize(
     libfsntfs_data_stream_t **data_stream,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_attribute_t *data_attribute,
     libcerror_error_t **error )
{
	libfsntfs_internal_data_stream_t *internal_data_stream = NULL;
	static char *function                                  = "libfsntfs_data_stream_initialize";

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	if( *data_stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data stream value already set.",
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
	internal_data_stream = memory_allocate_structure(
	                        libfsntfs_internal_data_stream_t );

	if( internal_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data stream.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_data_stream,
	     0,
	     sizeof( libfsntfs_internal_data_stream_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data stream.",
		 function );

		memory_free(
		 internal_data_stream );

		return( -1 );
	}
	if( libfsntfs_mft_attribute_get_data_extents_array(
	     data_attribute,
	     io_handle,
	     &( internal_data_stream->extents_array ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine extents array.",
		 function );

		goto on_error;
	}
	if( libfsntfs_cluster_block_stream_initialize(
	     &( internal_data_stream->data_cluster_block_stream ),
	     io_handle,
	     data_attribute,
	     NULL,
	     0,
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
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_data_stream->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_data_stream->file_io_handle = file_io_handle;
	internal_data_stream->data_attribute = data_attribute;

	*data_stream = (libfsntfs_data_stream_t *) internal_data_stream;

	return( 1 );

on_error:
	if( internal_data_stream != NULL )
	{
		if( internal_data_stream->data_cluster_block_stream != NULL )
		{
			libfdata_stream_free(
			 &( internal_data_stream->data_cluster_block_stream ),
			 NULL );
		}
		if( internal_data_stream->extents_array != NULL )
		{
			libcdata_array_free(
			 &( internal_data_stream->extents_array ),
			 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_extent_free,
			 NULL );
		}
		memory_free(
		 internal_data_stream );
	}
	return( -1 );
}

/* Frees a data stream
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_stream_free(
     libfsntfs_data_stream_t **data_stream,
     libcerror_error_t **error )
{
	libfsntfs_internal_data_stream_t *internal_data_stream = NULL;
	static char *function                                  = "libfsntfs_data_stream_free";
	int result                                             = 1;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	if( *data_stream != NULL )
	{
		internal_data_stream = (libfsntfs_internal_data_stream_t *) *data_stream;
		*data_stream         = NULL;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_data_stream->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		/* The file_io_handle and data_attribute references are freed elsewhere
		 */
		if( libfdata_stream_free(
		     &( internal_data_stream->data_cluster_block_stream ),
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
		if( libcdata_array_free(
		     &( internal_data_stream->extents_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_extent_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free extents array.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_data_stream );
	}
	return( result );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_stream_get_utf8_name_size(
     libfsntfs_data_stream_t *data_stream,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_data_stream_t *internal_data_stream = NULL;
	static char *function                                  = "libfsntfs_data_stream_get_utf8_name_size";
	int result                                             = 1;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	internal_data_stream = (libfsntfs_internal_data_stream_t *) data_stream;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_mft_attribute_get_utf8_name_size(
	     internal_data_stream->data_attribute,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 name from data stream.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_stream_get_utf8_name(
     libfsntfs_data_stream_t *data_stream,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_data_stream_t *internal_data_stream = NULL;
	static char *function                                  = "libfsntfs_data_stream_get_utf8_name";
	int result                                             = 1;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	internal_data_stream = (libfsntfs_internal_data_stream_t *) data_stream;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_mft_attribute_get_utf8_name(
	     internal_data_stream->data_attribute,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name from data stream.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_stream_get_utf16_name_size(
     libfsntfs_data_stream_t *data_stream,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_data_stream_t *internal_data_stream = NULL;
	static char *function                                  = "libfsntfs_data_stream_get_utf16_name_size";
	int result                                             = 1;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	internal_data_stream = (libfsntfs_internal_data_stream_t *) data_stream;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_mft_attribute_get_utf16_name_size(
	     internal_data_stream->data_attribute,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 name from data stream.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_stream_get_utf16_name(
     libfsntfs_data_stream_t *data_stream,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_data_stream_t *internal_data_stream = NULL;
	static char *function                                  = "libfsntfs_data_stream_get_utf16_name";
	int result                                             = 1;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	internal_data_stream = (libfsntfs_internal_data_stream_t *) data_stream;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_mft_attribute_get_utf16_name(
	     internal_data_stream->data_attribute,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name from data stream.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Reads data at the current offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsntfs_data_stream_read_buffer(
         libfsntfs_data_stream_t *data_stream,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libfsntfs_internal_data_stream_t *internal_data_stream = NULL;
	static char *function                                  = "libfsntfs_data_stream_read_buffer";
	ssize_t read_count                                     = 0;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	internal_data_stream = (libfsntfs_internal_data_stream_t *) data_stream;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	read_count = libfdata_stream_read_buffer(
	              internal_data_stream->data_cluster_block_stream,
	              (intptr_t *) internal_data_stream->file_io_handle,
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

		read_count = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsntfs_data_stream_read_buffer_at_offset(
         libfsntfs_data_stream_t *data_stream,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	libfsntfs_internal_data_stream_t *internal_data_stream = NULL;
	static char *function                                  = "libfsntfs_data_stream_read_buffer_at_offset";
	ssize_t read_count                                     = 0;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	internal_data_stream = (libfsntfs_internal_data_stream_t *) data_stream;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	read_count = libfdata_stream_read_buffer_at_offset(
	              internal_data_stream->data_cluster_block_stream,
	              (intptr_t *) internal_data_stream->file_io_handle,
	              buffer,
	              buffer_size,
	              offset,
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

		read_count = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Seeks a certain offset
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfsntfs_data_stream_seek_offset(
         libfsntfs_data_stream_t *data_stream,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libfsntfs_internal_data_stream_t *internal_data_stream = NULL;
	static char *function                                  = "libfsntfs_data_stream_seek_offset";

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	internal_data_stream = (libfsntfs_internal_data_stream_t *) data_stream;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	offset = libfdata_stream_seek_offset(
	          internal_data_stream->data_cluster_block_stream,
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

		offset = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( offset );
}

/* Retrieves the current offset
 * Returns the offset if successful or -1 on error
 */
int libfsntfs_data_stream_get_offset(
     libfsntfs_data_stream_t *data_stream,
     off64_t *offset,
     libcerror_error_t **error )
{
	libfsntfs_internal_data_stream_t *internal_data_stream = NULL;
	static char *function                                  = "libfsntfs_data_stream_get_offset";
	int result                                             = 1;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	internal_data_stream = (libfsntfs_internal_data_stream_t *) data_stream;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfdata_stream_get_offset(
	     internal_data_stream->data_cluster_block_stream,
	     offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve offset from data cluster block stream.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_stream_get_size(
     libfsntfs_data_stream_t *data_stream,
     size64_t *size,
     libcerror_error_t **error )
{
	libfsntfs_internal_data_stream_t *internal_data_stream = NULL;
	static char *function                                  = "libfsntfs_data_stream_get_size";
	int result                                             = 1;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	internal_data_stream = (libfsntfs_internal_data_stream_t *) data_stream;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfdata_stream_get_size(
	     internal_data_stream->data_cluster_block_stream,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size from data cluster block stream.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of extents (decoded data runs)
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_stream_get_number_of_extents(
     libfsntfs_data_stream_t *data_stream,
     int *number_of_extents,
     libcerror_error_t **error )
{
	libfsntfs_internal_data_stream_t *internal_data_stream = NULL;
	static char *function                                  = "libfsntfs_data_stream_get_number_of_extents";
	int result                                             = 1;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	internal_data_stream = (libfsntfs_internal_data_stream_t *) data_stream;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_array_get_number_of_entries(
	     internal_data_stream->extents_array,
	     number_of_extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of extents.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves a specific extent (decoded data run)
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_stream_get_extent_by_index(
     libfsntfs_data_stream_t *data_stream,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error )
{
	libfsntfs_extent_t *data_extent                        = NULL;
	libfsntfs_internal_data_stream_t *internal_data_stream = NULL;
	static char *function                                  = "libfsntfs_data_stream_get_extent_by_index";
	int result                                             = 1;

	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	internal_data_stream = (libfsntfs_internal_data_stream_t *) data_stream;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libcdata_array_get_entry_by_index(
	     internal_data_stream->extents_array,
	     extent_index,
	     (intptr_t **) &data_extent,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extent: %d.",
		 function,
		 extent_index );

		result = -1;
	}
	if( result == 1 )
	{
		if( libfsntfs_extent_get_values(
		     data_extent,
		     extent_offset,
		     extent_size,
		     extent_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extent: %d values.",
			 function,
			 extent_index );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_data_stream->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

