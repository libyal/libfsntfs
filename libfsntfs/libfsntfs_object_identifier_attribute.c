/*
 * Object identifier attribute ($OBJECT_ID) functions
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

#include "libfsntfs_attribute.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcthreads.h"
#include "libfsntfs_object_identifier_attribute.h"
#include "libfsntfs_object_identifier_values.h"
#include "libfsntfs_types.h"

/* Retrieves the droid file identifier
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_object_identifier_attribute_get_droid_file_identifier(
     libfsntfs_attribute_t *attribute,
     uint8_t *guid,
     size_t size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute             = NULL;
	libfsntfs_object_identifier_values_t *object_identifier_values = NULL;
	static char *function                                          = "libfsntfs_object_identifier_attribute_get_droid_file_identifier";
	uint32_t attribute_type                                        = 0;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( internal_attribute->value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attribute - missing value.",
		 function );

		return( -1 );
	}
	if( libfsntfs_internal_attribute_get_type(
	     internal_attribute,
	     &attribute_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute type.",
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
	object_identifier_values = (libfsntfs_object_identifier_values_t *) internal_attribute->value;

	if( guid == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid GUID.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: size is too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     guid,
	     object_identifier_values->droid_file_identifier,
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
	return( 1 );
}

/* Retrieves the birth droid volume identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_object_identifier_attribute_get_birth_droid_volume_identifier(
     libfsntfs_attribute_t *attribute,
     uint8_t *guid,
     size_t size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute             = NULL;
	libfsntfs_object_identifier_values_t *object_identifier_values = NULL;
	static char *function                                          = "libfsntfs_object_identifier_attribute_get_birth_droid_volume_identifier";
	uint64_t attribute_data_size                                   = 0;
	uint32_t attribute_type                                        = 0;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( internal_attribute->value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attribute - missing value.",
		 function );

		return( -1 );
	}
	if( libfsntfs_internal_attribute_get_type(
	     internal_attribute,
	     &attribute_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute type.",
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
	object_identifier_values = (libfsntfs_object_identifier_values_t *) internal_attribute->value;

	if( guid == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid GUID.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: size is too small.",
		 function );

		return( -1 );
	}
	if( libfsntfs_internal_attribute_get_data_size(
	     internal_attribute,
	     &attribute_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data from attribute.",
		 function );

		return( -1 );
	}
	if( attribute_data_size < 64 )
	{
		return( 0 );
	}
	if( memory_copy(
	     guid,
	     object_identifier_values->birth_droid_volume_identifier,
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
	return( 1 );
}

/* Retrieves the birth droid file identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_object_identifier_attribute_get_birth_droid_file_identifier(
     libfsntfs_attribute_t *attribute,
     uint8_t *guid,
     size_t size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute             = NULL;
	libfsntfs_object_identifier_values_t *object_identifier_values = NULL;
	static char *function                                          = "libfsntfs_object_identifier_attribute_get_birth_droid_file_identifier";
	uint64_t attribute_data_size                                   = 0;
	uint32_t attribute_type                                        = 0;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( internal_attribute->value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attribute - missing value.",
		 function );

		return( -1 );
	}
	if( libfsntfs_internal_attribute_get_type(
	     internal_attribute,
	     &attribute_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute type.",
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
	object_identifier_values = (libfsntfs_object_identifier_values_t *) internal_attribute->value;

	if( guid == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid GUID.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: size is too small.",
		 function );

		return( -1 );
	}
	if( libfsntfs_internal_attribute_get_data_size(
	     internal_attribute,
	     &attribute_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data from attribute.",
		 function );

		return( -1 );
	}
	if( attribute_data_size < 64 )
	{
		return( 0 );
	}
	if( memory_copy(
	     guid,
	     object_identifier_values->birth_droid_file_identifier,
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
	return( 1 );
}

/* Retrieves the birth droid domain identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_object_identifier_attribute_get_birth_droid_domain_identifier(
     libfsntfs_attribute_t *attribute,
     uint8_t *guid,
     size_t size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute             = NULL;
	libfsntfs_object_identifier_values_t *object_identifier_values = NULL;
	static char *function                                          = "libfsntfs_object_identifier_attribute_get_birth_droid_domain_identifier";
	uint64_t attribute_data_size                                   = 0;
	uint32_t attribute_type                                        = 0;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( internal_attribute->value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attribute - missing value.",
		 function );

		return( -1 );
	}
	if( libfsntfs_internal_attribute_get_type(
	     internal_attribute,
	     &attribute_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute type.",
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
	object_identifier_values = (libfsntfs_object_identifier_values_t *) internal_attribute->value;

	if( guid == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid GUID.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: size is too small.",
		 function );

		return( -1 );
	}
	if( libfsntfs_internal_attribute_get_data_size(
	     internal_attribute,
	     &attribute_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data from attribute.",
		 function );

		return( -1 );
	}
	if( attribute_data_size < 64 )
	{
		return( 0 );
	}
	if( memory_copy(
	     guid,
	     object_identifier_values->birth_droid_domain_identifier,
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
	return( 1 );
}

