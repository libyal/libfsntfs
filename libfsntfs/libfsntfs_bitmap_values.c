/*
 * Bitmap attribute ($BITMAP) functions
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

#include "libfsntfs_bitmap_values.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"

/* Creates bitmap values
 * Make sure the value bitmap_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_bitmap_values_initialize(
     libfsntfs_bitmap_values_t **bitmap_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_bitmap_values_initialize";

	if( bitmap_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bitmap values.",
		 function );

		return( -1 );
	}
	if( *bitmap_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid bitmap values value already set.",
		 function );

		return( -1 );
	}
	*bitmap_values = memory_allocate_structure(
	                  libfsntfs_bitmap_values_t );

	if( *bitmap_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create bitmap values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *bitmap_values,
	     0,
	     sizeof( libfsntfs_bitmap_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear bitmap values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *bitmap_values != NULL )
	{
		memory_free(
		 *bitmap_values );

		*bitmap_values = NULL;
	}
	return( -1 );
}

/* Frees bitmap values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_bitmap_values_free(
     libfsntfs_bitmap_values_t **bitmap_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_bitmap_values_free";
	int result            = 1;

	if( bitmap_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bitmap values.",
		 function );

		return( -1 );
	}
	if( *bitmap_values != NULL )
	{
		memory_free(
		 *bitmap_values );

		*bitmap_values = NULL;
	}
	return( result );
}

/* Reads the bitmap values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_bitmap_values_read(
     libfsntfs_bitmap_values_t *bitmap_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function             = "libfsntfs_bitmap_values_read";
	size_t data_offset                = 0;
	uint8_t bit_index                 = 0;
	uint8_t byte_value                = 0;
	uint8_t in_allocated_range        = 0;
	int allocated_element_index       = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	int first_allocated_element_index = 0;
#endif

	if( bitmap_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bitmap values.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: bitmap data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	while( data_offset < data_size )
	{
		byte_value = data[ data_offset ];

		data_offset++;

		for( bit_index = 0;
		     bit_index < 8;
		     bit_index++ )
		{
			if( in_allocated_range == 0 )
			{
				if( ( byte_value & 0x01 ) != 0 )
				{
					in_allocated_range            = 1;
#if defined( HAVE_DEBUG_OUTPUT )
					first_allocated_element_index = allocated_element_index;
#endif
				}
			}
			else if( ( byte_value & 0x01 ) == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( first_allocated_element_index == ( allocated_element_index - 1 ) )
					{
						libcnotify_printf(
						 "%s: allocated element\t\t\t\t: %d\n",
						 function,
						 first_allocated_element_index );
					}
					else
					{
						libcnotify_printf(
						 "%s: allocated element\t\t\t\t: %d - %d\n",
						 function,
						 first_allocated_element_index,
						 allocated_element_index - 1 );
					}
				}
#endif
				in_allocated_range = 0;
			}
			byte_value >>= 1;

			allocated_element_index++;
		}
	}
	if( in_allocated_range != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( first_allocated_element_index == ( allocated_element_index - 1 ) )
			{
				libcnotify_printf(
				 "%s: allocated element\t\t\t\t: %d\n",
				 function,
				 first_allocated_element_index );
			}
			else
			{
				libcnotify_printf(
				 "%s: allocated element\t\t\t\t: %d - %d\n",
				 function,
				 first_allocated_element_index,
				 allocated_element_index - 1 );
			}
		}
#endif
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

