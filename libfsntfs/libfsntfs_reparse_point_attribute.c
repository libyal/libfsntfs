/*
 * Reparse point attribute ($REPARSE_POINT) functions
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
#include "libfsntfs_reparse_point_attribute.h"
#include "libfsntfs_reparse_point_values.h"

/* Retrieves the tag
 * The tag is a combination of the type and flags
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_reparse_point_attribute_get_tag(
     libfsntfs_attribute_t *attribute,
     uint32_t *tag,
     libcerror_error_t **error )
{
	libfsntfs_reparse_point_values_t *reparse_point_values = NULL;
	libfsntfs_internal_attribute_t *internal_attribute     = NULL;
	static char *function                                  = "libfsntfs_reparse_point_attribute_get_tag";

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

	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
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
	reparse_point_values = (libfsntfs_reparse_point_values_t *) internal_attribute->value;

	if( tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid tag.",
		 function );

		return( -1 );
	}
	*tag = reparse_point_values->tag;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded substitute name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_attribute_get_utf8_substitute_name_size(
     libfsntfs_attribute_t *attribute,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_reparse_point_attribute_get_utf8_substitute_name_size";
	int result                                         = 0;

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

	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	result = libfsntfs_reparse_point_values_get_utf8_substitute_name_size(
	          (libfsntfs_reparse_point_values_t *) internal_attribute->value,
	          utf8_name_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 substitute name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-8 encoded substitute name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_attribute_get_utf8_substitute_name(
     libfsntfs_attribute_t *attribute,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_reparse_point_attribute_get_utf8_substitute_name";
	int result                                         = 0;

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

	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	result = libfsntfs_reparse_point_values_get_utf8_substitute_name(
	          (libfsntfs_reparse_point_values_t *) internal_attribute->value,
	          utf8_name,
	          utf8_name_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 substitute name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-16 encoded substitute name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_attribute_get_utf16_substitute_name_size(
     libfsntfs_attribute_t *attribute,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_reparse_point_attribute_get_utf16_substitute_name_size";
	int result                                         = 0;

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

	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	result = libfsntfs_reparse_point_values_get_utf16_substitute_name_size(
	          (libfsntfs_reparse_point_values_t *) internal_attribute->value,
	          utf16_name_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 substitute name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded substitute name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_attribute_get_utf16_substitute_name(
     libfsntfs_attribute_t *attribute,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_reparse_point_attribute_get_utf16_substitute_name";
	int result                                         = 0;

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

	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	result = libfsntfs_reparse_point_values_get_utf16_substitute_name(
	          (libfsntfs_reparse_point_values_t *) internal_attribute->value,
	          utf16_name,
	          utf16_name_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 substitute name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-8 encoded print name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_attribute_get_utf8_print_name_size(
     libfsntfs_attribute_t *attribute,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_reparse_point_attribute_get_utf8_print_name_size";
	int result                                         = 0;

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

	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	result = libfsntfs_reparse_point_values_get_utf8_print_name_size(
	          (libfsntfs_reparse_point_values_t *) internal_attribute->value,
	          utf8_name_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 print name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-8 encoded print name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_attribute_get_utf8_print_name(
     libfsntfs_attribute_t *attribute,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_reparse_point_attribute_get_utf8_print_name";
	int result                                         = 0;

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

	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	result = libfsntfs_reparse_point_values_get_utf8_print_name(
	          (libfsntfs_reparse_point_values_t *) internal_attribute->value,
	          utf8_name,
	          utf8_name_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 print name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-16 encoded print name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_attribute_get_utf16_print_name_size(
     libfsntfs_attribute_t *attribute,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_reparse_point_attribute_get_utf16_print_name_size";
	int result                                         = 0;

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

	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	result = libfsntfs_reparse_point_values_get_utf16_print_name_size(
	          (libfsntfs_reparse_point_values_t *) internal_attribute->value,
	          utf16_name_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 print name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded print name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_reparse_point_attribute_get_utf16_print_name(
     libfsntfs_attribute_t *attribute,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_reparse_point_attribute_get_utf16_print_name";
	int result                                         = 0;

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

	if( internal_attribute->type != LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		return( -1 );
	}
	result = libfsntfs_reparse_point_values_get_utf16_print_name(
	          (libfsntfs_reparse_point_values_t *) internal_attribute->value,
	          utf16_name,
	          utf16_name_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 print name.",
		 function );

		return( -1 );
	}
	return( result );
}

