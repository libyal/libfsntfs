/*
 * Volume name attribute ($VOLUME_NAME) functions
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

#include "libfsntfs_attribute.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_types.h"
#include "libfsntfs_volume_name_attribute.h"
#include "libfsntfs_volume_name_values.h"

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_name_attribute_get_utf8_name_size(
     libfsntfs_attribute_t *attribute,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_volume_name_attribute_get_utf8_name_size";

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

	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	if( libfsntfs_volume_name_values_get_utf8_name_size(
	     (libfsntfs_volume_name_values_t *) internal_attribute->value,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_name_attribute_get_utf8_name(
     libfsntfs_attribute_t *attribute,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_volume_name_attribute_get_utf8_name";

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

	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	if( libfsntfs_volume_name_values_get_utf8_name(
	     (libfsntfs_volume_name_values_t *) internal_attribute->value,
	     utf8_name,
	     utf8_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_name_attribute_get_utf16_name_size(
     libfsntfs_attribute_t *attribute,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_volume_name_attribute_get_utf16_name_size";

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

	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	if( libfsntfs_volume_name_values_get_utf16_name_size(
	     (libfsntfs_volume_name_values_t *) internal_attribute->value,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_name_attribute_get_utf16_name(
     libfsntfs_attribute_t *attribute,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_volume_name_attribute_get_utf16_name";

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

	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	if( libfsntfs_volume_name_values_get_utf16_name(
	     (libfsntfs_volume_name_values_t *) internal_attribute->value,
	     utf16_name,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

