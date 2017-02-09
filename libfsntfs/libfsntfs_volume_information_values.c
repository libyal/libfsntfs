/*
 * Volume information attribute ($VOLUME_INFORMATION) values functions
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

#include "libfsntfs_attribute.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_volume_information_values.h"

#include "fsntfs_volume_information.h"

/* Creates volume information values
 * Make sure the value volume_information_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_information_values_initialize(
     libfsntfs_volume_information_values_t **volume_information_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_volume_information_values_initialize";

	if( volume_information_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume information values.",
		 function );

		return( -1 );
	}
	if( *volume_information_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume information values value already set.",
		 function );

		return( -1 );
	}
	*volume_information_values = memory_allocate_structure(
	                              libfsntfs_volume_information_values_t );

	if( *volume_information_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume information values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *volume_information_values,
	     0,
	     sizeof( libfsntfs_volume_information_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume information values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *volume_information_values != NULL )
	{
		memory_free(
		 *volume_information_values );

		*volume_information_values = NULL;
	}
	return( -1 );
}

/* Frees volume information values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_information_values_free(
     libfsntfs_volume_information_values_t **volume_information_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_volume_information_values_free";

	if( volume_information_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume information values.",
		 function );

		return( -1 );
	}
	if( *volume_information_values != NULL )
	{
		memory_free(
		 *volume_information_values );

		*volume_information_values = NULL;
	}
	return( 1 );
}

/* Reads the volume information values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_information_values_read(
     libfsntfs_volume_information_values_t *volume_information_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_volume_information_values_read";

	if( volume_information_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume information values.",
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
		 "%s: volume information data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	if( data_size != 12 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported volume information data size: %" PRIzd "\n",
		 function,
		 data_size );

		return( -1 );
	}
	volume_information_values->major_version = ( (fsntfs_volume_information_t *) data )->major_version;
	volume_information_values->minor_version = ( (fsntfs_volume_information_t *) data )->minor_version;

	byte_stream_copy_to_uint16_little_endian(
	 ( (fsntfs_volume_information_t *) data )->flags,
	 volume_information_values->flags );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 ( (fsntfs_volume_information_t *) data )->unknown1,
		 8,
		 0 );

		libcnotify_printf(
		 "%s: major version\t\t\t: %" PRIu8 "\n",
		 function,
		 volume_information_values->major_version );

		libcnotify_printf(
		 "%s: minor version\t\t\t: %" PRIu8 "\n",
		 function,
		 volume_information_values->minor_version );

		libcnotify_printf(
		 "%s: flags\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 volume_information_values->flags );

		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

