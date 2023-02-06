/*
 * Object identifier attribute ($OBJECT_ID) values functions
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
#include <byte_stream.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libfguid.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_object_identifier_values.h"

#include "fsntfs_object_identifier.h"

/* Creates object identifier values
 * Make sure the value object_identifier_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_object_identifier_values_initialize(
     libfsntfs_object_identifier_values_t **object_identifier_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_object_identifier_values_initialize";

	if( object_identifier_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object identifier values.",
		 function );

		return( -1 );
	}
	if( *object_identifier_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid object identifier values values value already set.",
		 function );

		return( -1 );
	}
	*object_identifier_values = memory_allocate_structure(
	                             libfsntfs_object_identifier_values_t );

	if( *object_identifier_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create object identifier values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *object_identifier_values,
	     0,
	     sizeof( libfsntfs_object_identifier_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear object identifier values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *object_identifier_values != NULL )
	{
		memory_free(
		 *object_identifier_values );

		*object_identifier_values = NULL;
	}
	return( -1 );
}

/* Frees object identifier values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_object_identifier_values_free(
     libfsntfs_object_identifier_values_t **object_identifier_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_object_identifier_values_free";

	if( object_identifier_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object identifier values.",
		 function );

		return( -1 );
	}
	if( *object_identifier_values != NULL )
	{
		memory_free(
		 *object_identifier_values );

		*object_identifier_values = NULL;
	}
	return( 1 );
}

/* Reads the object identifier values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_object_identifier_values_read_data(
     libfsntfs_object_identifier_values_t *object_identifier_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_object_identifier_read_data";

	if( object_identifier_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object identifier values.",
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
		 "%s: object identifier data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	if( ( data_size != 16 )
	 && ( data_size != 64 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported object identifier data size: %" PRIzd "\n",
		 function,
		 data_size );

		return( -1 );
	}
	if( memory_copy(
	     object_identifier_values->droid_file_identifier,
	     ( (fsntfs_object_identifier_t *) data )->droid_file_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy droid file identifier.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libfsntfs_debug_print_guid_value(
		     function,
		     "droid file identifier\t\t\t",
		     object_identifier_values->droid_file_identifier,
		     16,
		     LIBFGUID_ENDIAN_LITTLE,
		     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print GUID value.",
			 function );

			return( -1 );
		}
	}
#endif
	if( data_size > 16 )
	{
		if( memory_copy(
		     object_identifier_values->birth_droid_volume_identifier,
		     ( (fsntfs_object_identifier_t *) data )->birth_droid_volume_identifier,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy birth droid volume identifier.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     object_identifier_values->birth_droid_file_identifier,
		     ( (fsntfs_object_identifier_t *) data )->birth_droid_file_identifier,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy birth droid file identifier.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     object_identifier_values->birth_droid_domain_identifier,
		     ( (fsntfs_object_identifier_t *) data )->birth_droid_domain_identifier,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy birth droid domain identifier.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libfsntfs_debug_print_guid_value(
			     function,
			     "birth droid volume identifier\t\t",
			     object_identifier_values->birth_droid_volume_identifier,
			     16,
			     LIBFGUID_ENDIAN_LITTLE,
			     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print GUID value.",
				 function );

				return( -1 );
			}
			if( libfsntfs_debug_print_guid_value(
			     function,
			     "birth droid file identifier\t\t",
			     object_identifier_values->birth_droid_file_identifier,
			     16,
			     LIBFGUID_ENDIAN_LITTLE,
			     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print GUID value.",
				 function );

				return( -1 );
			}
			if( libfsntfs_debug_print_guid_value(
			     function,
			     "birth droid domain identifier\t\t",
			     object_identifier_values->birth_droid_domain_identifier,
			     16,
			     LIBFGUID_ENDIAN_LITTLE,
			     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print GUID value.",
				 function );

				return( -1 );
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

/* Reads the object identifier values from an MFT attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_object_identifier_values_read_from_mft_attribute(
     libfsntfs_object_identifier_values_t *object_identifier_values,
     libfsntfs_mft_attribute_t *mft_attribute,
     libcerror_error_t **error )
{
	uint8_t *data           = NULL;
	static char *function   = "libfsntfs_object_identifier_values_read_from_mft_attribute";
	size_t data_size        = 0;
	uint32_t attribute_type = 0;
	int result              = 0;

	if( object_identifier_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object identifier values.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_attribute_get_type(
	     mft_attribute,
	     &attribute_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve type from attribute.",
		 function );

		return( -1 );
	}
	if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	result = libfsntfs_mft_attribute_data_is_resident(
	          mft_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if attribute data is resident.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported non-resident attribute.",
		 function );

		return( 1 );
	}
	if( libfsntfs_mft_attribute_get_resident_data(
	     mft_attribute,
	     &data,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve resident data from attribute.",
		 function );

		return( -1 );
	}
	if( libfsntfs_object_identifier_values_read_data(
	     object_identifier_values,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read object identifier values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

