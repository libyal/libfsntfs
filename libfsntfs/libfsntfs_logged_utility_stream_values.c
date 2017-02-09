/*
 * Logged utility stream attribute ($LOGGED_UTILITY_STREAM) values functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_logged_utility_stream_values.h"

#include "fsntfs_logged_utility_stream.h"

/* Creates logged utility stream values
 * Make sure the value logged_utility_stream_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_logged_utility_stream_values_initialize(
     libfsntfs_logged_utility_stream_values_t **logged_utility_stream_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_logged_utility_stream_values_initialize";

	if( logged_utility_stream_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logged utility stream values.",
		 function );

		return( -1 );
	}
	if( *logged_utility_stream_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid logged utility stream values value already set.",
		 function );

		return( -1 );
	}
	*logged_utility_stream_values = memory_allocate_structure(
	                                 libfsntfs_logged_utility_stream_values_t );

	if( *logged_utility_stream_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create logged utility stream values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *logged_utility_stream_values,
	     0,
	     sizeof( libfsntfs_logged_utility_stream_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear logged utility stream values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *logged_utility_stream_values != NULL )
	{
		memory_free(
		 *logged_utility_stream_values );

		*logged_utility_stream_values = NULL;
	}
	return( -1 );
}

/* Frees logged utility stream values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_logged_utility_stream_values_free(
     libfsntfs_logged_utility_stream_values_t **logged_utility_stream_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_logged_utility_stream_values_free";

	if( logged_utility_stream_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logged utility stream values.",
		 function );

		return( -1 );
	}
	if( *logged_utility_stream_values != NULL )
	{
		memory_free(
		 *logged_utility_stream_values );

		*logged_utility_stream_values = NULL;
	}
	return( 1 );
}

/* Reads the logged utility stream values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_logged_utility_stream_values_read(
     libfsntfs_logged_utility_stream_values_t *logged_utility_stream_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_logged_utility_stream_values_read";

	if( logged_utility_stream_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logged utility stream values.",
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
		 "%s: logged utility stream data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	return( 1 );
}

