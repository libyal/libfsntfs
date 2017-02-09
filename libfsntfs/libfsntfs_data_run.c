/*
 * Data run functions
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

#include "libfsntfs_data_run.h"
#include "libfsntfs_libcerror.h"

/* Creates a data run
 * Make sure the value data_run is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_run_initialize(
     libfsntfs_data_run_t **data_run,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_data_run_initialize";

	if( data_run == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data run.",
		 function );

		return( -1 );
	}
	if( *data_run != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data run value already set.",
		 function );

		return( -1 );
	}
	*data_run = memory_allocate_structure(
	             libfsntfs_data_run_t );

	if( *data_run == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data run.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_run,
	     0,
	     sizeof( libfsntfs_data_run_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data run.",
		 function );

		memory_free(
		 *data_run );

		*data_run = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *data_run != NULL )
	{
		memory_free(
		 *data_run );

		*data_run = NULL;
	}
	return( -1 );
}

/* Frees a data run
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_run_free(
     libfsntfs_data_run_t **data_run,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_data_run_free";

	if( data_run == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data run.",
		 function );

		return( -1 );
	}
	if( *data_run != NULL )
	{
		memory_free(
		 *data_run );

		*data_run = NULL;
	}
	return( 1 );
}

