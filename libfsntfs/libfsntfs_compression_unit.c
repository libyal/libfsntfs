/*
 * Compression unit functions
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

#include "libfsntfs_compression_unit.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_libfwnt.h"
#include "libfsntfs_types.h"
#include "libfsntfs_unused.h"

/* Creates a compression unit
 * Make sure the value compression_unit is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compression_unit_initialize(
     libfsntfs_compression_unit_t **compression_unit,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compression_unit_initialize";

	if( compression_unit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression unit.",
		 function );

		return( -1 );
	}
	if( *compression_unit != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid compression unit value already set.",
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
	*compression_unit = memory_allocate_structure(
	                     libfsntfs_compression_unit_t );

	if( *compression_unit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compression unit.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *compression_unit,
	     0,
	     sizeof( libfsntfs_compression_unit_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear compression unit.",
		 function );

		memory_free(
		 *compression_unit );

		*compression_unit = NULL;

		return( -1 );
	}
	if( data_size > 0 )
	{
		( *compression_unit )->data = (uint8_t *) memory_allocate(
		                                           sizeof( uint8_t ) * data_size );

		if( ( *compression_unit )->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		( *compression_unit )->data_size = data_size;
	}
	return( 1 );

on_error:
	if( *compression_unit != NULL )
	{
		memory_free(
		 *compression_unit );

		*compression_unit = NULL;
	}
	return( -1 );
}

/* Frees a compression unit
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compression_unit_free(
     libfsntfs_compression_unit_t **compression_unit,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_compression_unit_free";

	if( compression_unit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compression unit.",
		 function );

		return( -1 );
	}
	if( *compression_unit != NULL )
	{
		if( ( *compression_unit )->data != NULL )
		{
			memory_free(
			 ( *compression_unit )->data );
		}
		memory_free(
		 *compression_unit );

		*compression_unit = NULL;
	}
	return( 1 );
}

/* Reads a compression unit
 * Callback function for the compression unit list
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_compression_unit_read_element_data(
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *list_element,
     libfcache_cache_t *cache,
     int element_data_file_index LIBFSNTFS_ATTRIBUTE_UNUSED,
     off64_t compression_unit_data_offset,
     size64_t compression_unit_data_size,
     uint32_t range_flags,
     uint8_t read_flags LIBFSNTFS_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfsntfs_compression_unit_t *compression_unit = NULL;
	uint8_t *compressed_data                       = NULL;
	uint8_t *compression_unit_data                 = NULL;
	static char *function                          = "libfsntfs_compression_unit_read_element_data";
	size64_t compression_unit_size                 = 0;
	ssize_t read_count                             = 0;
	int result                                     = 0;

	LIBFSNTFS_UNREFERENCED_PARAMETER( element_data_file_index )
	LIBFSNTFS_UNREFERENCED_PARAMETER( read_flags )

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
	if( compression_unit_data_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid compression unit data size value out of bounds.",
		 function );

		return( -1 );
	}
	result = libfdata_list_element_get_mapped_size(
	          list_element,
	          &compression_unit_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of name string.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		compression_unit_size = compression_unit_data_size;
	}
	if( compression_unit_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid compression unit size value out of bounds.",
		 function );

		goto on_error;
	}
	if( libfsntfs_compression_unit_initialize(
	     &compression_unit,
	     compression_unit_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create compression unit.",
		 function );

		goto on_error;
	}
	if( compression_unit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing compression unit.",
		 function );

		goto on_error;
	}
	if( ( range_flags & LIBFDATA_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		if( memory_set(
		     compression_unit->data,
		     0,
		     compression_unit->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear compression unit data.",
			 function );

			goto on_error;
		}
	}
	else
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading compression unit at offset: 0x%08" PRIx64 " with size: %" PRIu64 ".\n",
			 function,
			 compression_unit_data_offset,
			 compression_unit_data_size );
		}
#endif
		if( libbfio_handle_seek_offset(
		     file_io_handle,
		     compression_unit_data_offset,
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset: 0x%08" PRIx64 ".",
			 function,
			 compression_unit_data_offset );

			goto on_error;
		}
		if( ( range_flags & LIBFDATA_RANGE_FLAG_IS_COMPRESSED ) != 0 )
		{
			compressed_data = (uint8_t *) memory_allocate(
			                               sizeof( uint8_t ) * (size_t) compression_unit_data_size );

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
			compression_unit_data = compressed_data;
		}
		else
		{
			compression_unit_data = compression_unit->data;
		}
		read_count = libbfio_handle_read_buffer(
		              file_io_handle,
		              compression_unit_data,
		              (size_t) compression_unit_data_size,
		              error );

		if( read_count != (ssize_t) compression_unit_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read compression unit.",
			 function );

			goto on_error;
		}
		if( ( range_flags & LIBFDATA_RANGE_FLAG_IS_COMPRESSED ) != 0 )
		{
			result = libfwnt_lznt1_decompress(
			          compressed_data,
			          (size_t) compression_unit_data_size,
			          compression_unit->data,
			          &( compression_unit->data_size ),
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
	}
	if( libfdata_list_element_set_element_value(
	     list_element,
	     (intptr_t *) file_io_handle,
	     cache,
	     (intptr_t *) compression_unit,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_compression_unit_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set compression unit as element value.",
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
	if( compression_unit != NULL )
	{
		libfsntfs_compression_unit_free(
		 &compression_unit,
		 NULL );
	}
	return( -1 );
}

