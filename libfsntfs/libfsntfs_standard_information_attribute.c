/*
 * Standard information attribute ($STANDARD_INFORMATION) functions
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
#include "libfsntfs_standard_information_attribute.h"
#include "libfsntfs_standard_information_values.h"
#include "libfsntfs_types.h"

/* Retrieves the creation date and time
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_standard_information_attribute_get_creation_time(
     libfsntfs_attribute_t *attribute,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute                   = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_standard_information_attribute_get_creation_time";
	uint32_t attribute_type                                              = 0;

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
	if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	standard_information_values = (libfsntfs_standard_information_values_t *) internal_attribute->value;

	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid FILETIME.",
		 function );

		return( -1 );
	}
	*filetime = standard_information_values->creation_time;

	return( 1 );
}

/* Retrieves the (file) modification (last written) date and time
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_standard_information_attribute_get_modification_time(
     libfsntfs_attribute_t *attribute,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute                   = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_standard_information_attribute_get_modification_time";
	uint32_t attribute_type                                              = 0;

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
	if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	standard_information_values = (libfsntfs_standard_information_values_t *) internal_attribute->value;

	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid FILETIME.",
		 function );

		return( -1 );
	}
	*filetime = standard_information_values->modification_time;

	return( 1 );
}

/* Retrieves the access date and time
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_standard_information_attribute_get_access_time(
     libfsntfs_attribute_t *attribute,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute                   = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_standard_information_attribute_get_access_time";
	uint32_t attribute_type                                              = 0;

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
	if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	standard_information_values = (libfsntfs_standard_information_values_t *) internal_attribute->value;

	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid FILETIME.",
		 function );

		return( -1 );
	}
	*filetime = standard_information_values->access_time;

	return( 1 );
}

/* Retrieves the (file system entry) modification date and time
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_standard_information_attribute_get_entry_modification_time(
     libfsntfs_attribute_t *attribute,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute                   = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_standard_information_attribute_get_entry_modification_time";
	uint32_t attribute_type                                              = 0;

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
	if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	standard_information_values = (libfsntfs_standard_information_values_t *) internal_attribute->value;

	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid FILETIME.",
		 function );

		return( -1 );
	}
	*filetime = standard_information_values->entry_modification_time;

	return( 1 );
}

/* Retrieves the file attribute flags
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_standard_information_attribute_get_file_attribute_flags(
     libfsntfs_attribute_t *attribute,
     uint32_t *file_attribute_flags,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute                   = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_standard_information_attribute_get_file_attribute_flags";
	uint32_t attribute_type                                              = 0;

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
	if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	standard_information_values = (libfsntfs_standard_information_values_t *) internal_attribute->value;

	if( file_attribute_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file attribute flags.",
		 function );

		return( -1 );
	}
	*file_attribute_flags = standard_information_values->file_attribute_flags;

	return( 1 );
}

/* Retrieves the owner identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_standard_information_attribute_get_owner_identifier(
     libfsntfs_attribute_t *attribute,
     uint32_t *owner_identifier,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute                   = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_standard_information_attribute_get_owner_identifier";
	uint64_t attribute_data_size                                         = 0;
	uint32_t attribute_type                                              = 0;

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
	if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	standard_information_values = (libfsntfs_standard_information_values_t *) internal_attribute->value;

	if( owner_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid owner identifier.",
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
	if( attribute_data_size < 72 )
	{
		return( 0 );
	}
	*owner_identifier = standard_information_values->owner_identifier;

	return( 1 );
}

/* Retrieves the security descriptor identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_standard_information_attribute_get_security_descriptor_identifier(
     libfsntfs_attribute_t *attribute,
     uint32_t *security_descriptor_identifier,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute                   = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_standard_information_attribute_get_security_descriptor_identifier";
	uint64_t attribute_data_size                                         = 0;
	uint32_t attribute_type                                              = 0;

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
	if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	standard_information_values = (libfsntfs_standard_information_values_t *) internal_attribute->value;

	if( security_descriptor_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor identifier.",
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
	if( attribute_data_size < 72 )
	{
		return( 0 );
	}
	*security_descriptor_identifier = standard_information_values->security_descriptor_identifier;

	return( 1 );
}

/* Retrieves the update sequence number (USN)
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_standard_information_attribute_get_update_sequence_number(
     libfsntfs_attribute_t *attribute,
     uint64_t *update_sequence_number,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute                   = NULL;
	libfsntfs_standard_information_values_t *standard_information_values = NULL;
	static char *function                                                = "libfsntfs_standard_information_attribute_get_update_sequence_number";
	uint64_t attribute_data_size                                         = 0;
	uint32_t attribute_type                                              = 0;

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
	if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	standard_information_values = (libfsntfs_standard_information_values_t *) internal_attribute->value;

	if( update_sequence_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid update sequence number (USN).",
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
	if( attribute_data_size < 72 )
	{
		return( 0 );
	}
	*update_sequence_number = standard_information_values->update_sequence_number;

	return( 1 );
}

