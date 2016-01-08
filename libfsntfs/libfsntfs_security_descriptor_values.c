/*
 * Security descriptor attribute ($SECURITY_DESCRIPTOR) values functions
 *
 * Copyright (C) 2010-2016, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfwnt.h"
#include "libfsntfs_security_descriptor_values.h"

/* Creates security descriptor values
 * Make sure the value security_descriptor_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_security_descriptor_values_initialize(
     libfsntfs_security_descriptor_values_t **security_descriptor_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_values_initialize";

	if( security_descriptor_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor values.",
		 function );

		return( -1 );
	}
	if( *security_descriptor_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid security descriptor values value already set.",
		 function );

		return( -1 );
	}
	*security_descriptor_values = memory_allocate_structure(
	                               libfsntfs_security_descriptor_values_t );

	if( *security_descriptor_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create security descriptor values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *security_descriptor_values,
	     0,
	     sizeof( libfsntfs_security_descriptor_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear security descriptor values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *security_descriptor_values != NULL )
	{
		memory_free(
		 *security_descriptor_values );

		*security_descriptor_values = NULL;
	}
	return( -1 );
}

/* Frees security descriptor values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_security_descriptor_values_free(
     libfsntfs_security_descriptor_values_t **security_descriptor_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_values_free";

	if( security_descriptor_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor values.",
		 function );

		return( -1 );
	}
	if( *security_descriptor_values != NULL )
	{
		memory_free(
		 *security_descriptor_values );

		*security_descriptor_values = NULL;
	}
	return( 1 );
}

/* Reads the security descriptor values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_security_descriptor_values_read(
     libfsntfs_security_descriptor_values_t *security_descriptor_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_values_read";

	if( security_descriptor_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor values.",
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
		 "%s: security descriptor data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	/* TODO use libfwnt to print the security descriptor */

	return( 1 );
}

